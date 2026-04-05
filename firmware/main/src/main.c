#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/rtc_io.h>
#include <esp_sleep.h>
#include <esp_timer.h>
#include <sdkconfig.h>
#include <nvs_flash.h>
#include <esp_log.h>

#include "bme280adaptor.h"
#include "i2cdevice.h"
#include "alarmnvs.h"
#include "battery.h"
#include "desired_door_state.h"
#include "sigfox.h"
#include "rv3029.h"
#include "ina219.h"
#include "buzzer.h"
#include "motor.h"
#include "solar.h"
#include "input.h"
#include "ota.h"
#include "runtime_status.h"

extern int rtc_wake_stub_force_link(void);
extern motor_state_t wake_stub_motor_request;

#define MOTOR_ENDSTATE_POWER_THRESHOLD_W 0.1f
#define MOTOR_ENDSTATE_MIN_DURATION_US 500000
#define MOTOR_REED_MIN_DURATION_US 500000
#define USER_INTERACTION_WINDOW_US 20000000LL
#define USER_LONG_PRESS_THRESHOLD_US 500000LL

static bool motor_state_is_active(motor_state_t state)
{
	return state == MOTOR_UP || state == MOTOR_DOWN ||
			state == MOTOR_UP_UNTIL_LATCH || state == MOTOR_DOWN_UNTIL_LATCH;
}

static motor_state_t button_state_to_direct_motor(button_state_t button_state)
{
	if (button_state == BUTTON_UP)
		return MOTOR_UP;
	if (button_state == BUTTON_DOWN)
		return MOTOR_DOWN;
	return MOTOR_OFF;
}

static motor_state_t button_state_to_latched_motor(button_state_t button_state)
{
	if (button_state == BUTTON_UP)
		return MOTOR_UP_UNTIL_LATCH;
	if (button_state == BUTTON_DOWN)
		return MOTOR_DOWN_UNTIL_LATCH;
	return MOTOR_OFF;
}

static bool motor_state_is_latched(motor_state_t state)
{
	return state == MOTOR_UP_UNTIL_LATCH || state == MOTOR_DOWN_UNTIL_LATCH;
}

static bool autoclose_is_due(uint8_t autoclose_hours, uint8_t time_hours, uint8_t alarm_minutes, uint8_t time_minutes)
{
	return autoclose_hours == time_hours && alarm_minutes == time_minutes;
}

static void disable_autoclose(void)
{
	ESP_ERROR_CHECK(alarmnvs_store_autoclose_hours(25));
	ESP_LOGI("main", "Autoclose executed, disabling one-shot autoclose by storing 25");
}

esp_err_t nvs_init(void)
{
	esp_err_t ret = nvs_flash_init();

	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}

	return ret;
}

static void motor_set_logged(motor_state_t state)
{
	static motor_state_t last_state = MOTOR_OFF;
	esp_err_t res = ESP_OK;

	if (state != last_state) {
		if (state == MOTOR_UP || state == MOTOR_UP_UNTIL_LATCH) {
			printf("[motor] activating up\r\n");
		} else if (state == MOTOR_DOWN || state == MOTOR_DOWN_UNTIL_LATCH) {
			printf("[motor] activating down\r\n");
		}

		if (state == MOTOR_UP || state == MOTOR_DOWN ||
				state == MOTOR_UP_UNTIL_LATCH || state == MOTOR_DOWN_UNTIL_LATCH) {
			res = desired_door_state_store_down_reached(false);
			if (res != ESP_OK)
				printf("[desired] failed to clear down-reached flag: %s\r\n", esp_err_to_name(res));
		}

		last_state = state;
	}

	motor_set(state);
}

static bool motor_power_end_state_reached_if_due(motor_state_t state, int64_t *last_log_time_us, int64_t *low_power_since_us)
{
	if (state != MOTOR_UP && state != MOTOR_DOWN &&
			state != MOTOR_UP_UNTIL_LATCH && state != MOTOR_DOWN_UNTIL_LATCH) {
		*last_log_time_us = 0;
		*low_power_since_us = 0;
		return false;
	}

	int64_t now = esp_timer_get_time();
	if (*last_log_time_us != 0 && now - *last_log_time_us < 100000)
		return false;

	float current_amps;
	float battery_voltage = battery_get_voltage();
	esp_err_t res = ina219_current(&current_amps);
	bool reed_closed = input_get_reed_state();
	bool end_state_reached = false;
	if (res == ESP_OK) {
		float power_watts = battery_voltage * current_amps;
		printf("[motor] power %.3f W, reed %s\r\n", power_watts, reed_closed ? "closed" : "open");

		if (power_watts < MOTOR_ENDSTATE_POWER_THRESHOLD_W) {
			if (*low_power_since_us == 0)
				*low_power_since_us = now;

			if (now - *low_power_since_us >= MOTOR_ENDSTATE_MIN_DURATION_US)
				end_state_reached = true;
		} else {
			*low_power_since_us = 0;
		}
	} else {
		printf("[motor] power read failed: %s, reed %s\r\n", esp_err_to_name(res), reed_closed ? "closed" : "open");
		*low_power_since_us = 0;
	}

	*last_log_time_us = now;
	return end_state_reached;
}

static bool motor_reed_stop_reached_if_due(motor_state_t state, int64_t *reed_closed_since_us)
{
	if (state != MOTOR_UP && state != MOTOR_UP_UNTIL_LATCH) {
		*reed_closed_since_us = 0;
		return false;
	}

	int64_t now = esp_timer_get_time();
	if (input_get_reed_state()) {
		if (*reed_closed_since_us == 0)
			*reed_closed_since_us = now;

		return now - *reed_closed_since_us >= MOTOR_REED_MIN_DURATION_US;
	}

	*reed_closed_since_us = 0;
	return false;
}

static void motor_stop_after_power_end_state(motor_state_t state)
{
	printf("[motor] end state reached via low power\r\n");
	if (state == MOTOR_DOWN || state == MOTOR_DOWN_UNTIL_LATCH) {
		ESP_ERROR_CHECK(desired_door_state_store_down_reached(true));
		printf("[desired] persisted DOWN reached flag\r\n");
	}
	motor_set_logged(MOTOR_BRAKE);
	buzzer_buzz(2000, 200);
}

static bool motor_run_until_target(motor_state_t state, int64_t timeout_s, desired_door_state_t desired_state)
{
	int64_t last_power_log_time_us = 0;
	int64_t low_power_since_us = 0;
	int64_t reed_closed_since_us = 0;
	int64_t start_time_us = esp_timer_get_time();
	int64_t timeout_us = timeout_s * 1000000LL;
	int64_t deadline_us = start_time_us + timeout_us;

	printf("[motor] moving toward desired state %s with timeout %lld s\r\n",
			desired_door_state_to_string(desired_state), timeout_s);
	motor_set_logged(state);

	while (esp_timer_get_time() < deadline_us) {
		if (desired_state == DESIRED_DOOR_STATE_UP && motor_reed_stop_reached_if_due(state, &reed_closed_since_us)) {
			printf("[motor] desired UP reached via reed contact\r\n");
			motor_set_logged(MOTOR_BRAKE);
			buzzer_buzz(2000, 200);
			return true;
		}

		if (motor_power_end_state_reached_if_due(state, &last_power_log_time_us, &low_power_since_us)) {
			motor_stop_after_power_end_state(state);
			return true;
		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}

	printf("[motor] timeout while moving toward desired state %s after %.3f s (configured %lld s)\r\n",
			desired_door_state_to_string(desired_state),
			(double) (esp_timer_get_time() - start_time_us) / 1000000.0,
			timeout_s);
	motor_set_logged(MOTOR_BRAKE);
	return false;
}

void app_main(void)
{
	// Determine wakeup reason
	ESP_ERROR_CHECK(input_init());
	wakeup_reason_t wakeup_reason = input_get_wakeup_reason();
	runtime_status_set_last_wakeup_reason(wakeup_reason);
	ESP_LOGI("main", "Wakeup reason resolved to %s",
			wakeup_reason == WAKEUP_REASON_USER ? "USER" :
			wakeup_reason == WAKEUP_REASON_RTC ? "RTC" : "UNKNOWN");

	// Initialize all systems
	ESP_ERROR_CHECK(ota_init());
	ESP_ERROR_CHECK(nvs_init());
	ESP_ERROR_CHECK(i2cdevice_init());

	ESP_ERROR_CHECK(bme280adaptor_init());
	ESP_ERROR_CHECK(ina219_init(INA219_MODE_CONT_SH_BUS, INA219_FSR_32V, INA219_PGA_80MV, INA219_ADC_12BIT, INA219_ADC_12BIT));
	ESP_ERROR_CHECK(rv3029_init());
	ESP_ERROR_CHECK(rv3029_set_testing_mode(false));
	ESP_ERROR_CHECK(buzzer_init());
	ESP_ERROR_CHECK(motor_init());
	ESP_ERROR_CHECK(battery_init());
	ESP_ERROR_CHECK(solar_init());
	ESP_ERROR_CHECK(sigfox_init());

	// set desired door state to down
	uint8_t alarm_minutes, alarm_hours, autoclose_hours;
	ESP_ERROR_CHECK(alarmnvs_retrieve_time(&alarm_minutes, &alarm_hours));
	ESP_ERROR_CHECK(alarmnvs_retrieve_autoclose_hours(&autoclose_hours));
	ESP_ERROR_CHECK(rv3029_set_alarm_minutes(alarm_minutes));
	ESP_LOGI("main", "Alarm set for %02d:%02d, autoclose at %02d:00", alarm_hours, alarm_minutes, autoclose_hours);

	uint8_t time_seconds, time_minutes, time_hours;
	ESP_ERROR_CHECK(rv3029_gettime(&time_seconds, &time_minutes, &time_hours));
	ESP_LOGI("main", "Current time is %02d:%02d:%02d", time_hours, time_minutes, time_seconds);

	desired_door_state_t desired_state;
	ESP_ERROR_CHECK(desired_door_state_retrieve(&desired_state));
	ESP_LOGI("main", "Desired door state before wake processing: %s", desired_door_state_to_string(desired_state));

	if (wakeup_reason == WAKEUP_REASON_USER) {
		desired_state = DESIRED_DOOR_STATE_USER;
		ESP_ERROR_CHECK(desired_door_state_store(desired_state));
		ESP_LOGI("main", "Wake reason is USER, storing desired door state USER");
	} else if (alarm_hours == time_hours && alarm_minutes == time_minutes) {
		desired_state = DESIRED_DOOR_STATE_UP;
		ESP_ERROR_CHECK(desired_door_state_store(desired_state));
		ESP_LOGI("main", "Alarm time matched, storing desired door state UP");
	} else if (autoclose_is_due(autoclose_hours, time_hours, alarm_minutes, time_minutes)) {
		desired_state = DESIRED_DOOR_STATE_DOWN;
		ESP_ERROR_CHECK(desired_door_state_store(desired_state));
		disable_autoclose();
		ESP_LOGI("main", "Autoclose time matched, storing desired door state DOWN");
	} else {
		ESP_LOGI("main", "No desired door state update on this wake");
	}

	ESP_LOGI("main", "Desired door state after wake processing: %s", desired_door_state_to_string(desired_state));

	// some devices (pressure sensor, INA219) need some time to initialize...
	vTaskDelay(200 / portTICK_PERIOD_MS);

	// User pressed button: drive motor
	if (wakeup_reason == WAKEUP_REASON_USER) {
		int64_t stay_awake_until_us = esp_timer_get_time() + USER_INTERACTION_WINDOW_US;
		button_state_t tracked_press = BUTTON_NONE;
		button_state_t suppressed_buttons = BUTTON_NONE;
		int64_t tracked_press_start_us = 0;
		bool both_action_handled = false;
		button_state_t raw_button_state = input_get_button_state();
		button_state_t button_state = raw_button_state;
		motor_state_t commanded_state = MOTOR_OFF;
		bool stopping_latched_motion = false;
		int64_t last_power_log_time_us = 0;
		int64_t low_power_since_us = 0;
		int64_t reed_closed_since_us = 0;

		if (button_state == BUTTON_NONE &&
				(wake_stub_motor_request == MOTOR_UP_UNTIL_LATCH || wake_stub_motor_request == MOTOR_DOWN_UNTIL_LATCH)) {
			commanded_state = wake_stub_motor_request;
			motor_set_logged(commanded_state);
		}
		wake_stub_motor_request = MOTOR_OFF;

		while (esp_timer_get_time() < stay_awake_until_us || motor_state_is_active(commanded_state)) {
			int64_t now_us = esp_timer_get_time();
			raw_button_state = input_get_button_state();

			if (suppressed_buttons != BUTTON_NONE && (raw_button_state & suppressed_buttons) == 0) {
				suppressed_buttons = BUTTON_NONE;
			}
			button_state = raw_button_state & ~suppressed_buttons;

			if (button_state != BUTTON_NONE) {
				stay_awake_until_us = now_us + USER_INTERACTION_WINDOW_US;
			}

			if (tracked_press == BUTTON_NONE) {
				if (button_state == BUTTON_UP || button_state == BUTTON_DOWN) {
					tracked_press = button_state;
					tracked_press_start_us = now_us;
					if (motor_state_is_latched(commanded_state)) {
						stopping_latched_motion = true;
						commanded_state = MOTOR_BRAKE;
						motor_set_logged(commanded_state);
					} else {
						commanded_state = button_state_to_direct_motor(button_state);
						if (!(commanded_state == MOTOR_UP && motor_reed_stop_reached_if_due(commanded_state, &reed_closed_since_us))) {
							motor_set_logged(commanded_state);
						}
					}
				} else if (button_state == BUTTON_BOTH) {
					tracked_press = BUTTON_BOTH;
					tracked_press_start_us = now_us;
					both_action_handled = false;
					stopping_latched_motion = false;
					commanded_state = MOTOR_BRAKE;
					motor_set_logged(commanded_state);
				} else if (motor_state_is_active(commanded_state)) {
					motor_set_logged(commanded_state);
				} else if (commanded_state == MOTOR_BRAKE) {
					commanded_state = MOTOR_OFF;
					motor_set_logged(commanded_state);
				}
			} else if (tracked_press == BUTTON_UP || tracked_press == BUTTON_DOWN) {
				if (stopping_latched_motion) {
					if (button_state == tracked_press) {
						if (now_us - tracked_press_start_us >= USER_LONG_PRESS_THRESHOLD_US) {
							stopping_latched_motion = false;
							commanded_state = button_state_to_direct_motor(tracked_press);
							if (!(commanded_state == MOTOR_UP && motor_reed_stop_reached_if_due(commanded_state, &reed_closed_since_us))) {
								motor_set_logged(commanded_state);
							}
						} else {
							commanded_state = MOTOR_BRAKE;
							motor_set_logged(commanded_state);
						}
					} else if (button_state == BUTTON_NONE) {
						commanded_state = MOTOR_BRAKE;
						motor_set_logged(commanded_state);
						tracked_press = BUTTON_NONE;
						stopping_latched_motion = false;
					} else if (button_state == BUTTON_BOTH) {
						tracked_press = BUTTON_BOTH;
						tracked_press_start_us = now_us;
						both_action_handled = false;
						stopping_latched_motion = false;
						commanded_state = MOTOR_BRAKE;
						motor_set_logged(commanded_state);
					} else {
						tracked_press = button_state;
						tracked_press_start_us = now_us;
						commanded_state = MOTOR_BRAKE;
						motor_set_logged(commanded_state);
					}
				} else if (button_state == tracked_press) {
					commanded_state = button_state_to_direct_motor(tracked_press);
					if (!(commanded_state == MOTOR_UP && motor_reed_stop_reached_if_due(commanded_state, &reed_closed_since_us))) {
						motor_set_logged(commanded_state);
					}
				} else if (button_state == BUTTON_NONE) {
					if (now_us - tracked_press_start_us < USER_LONG_PRESS_THRESHOLD_US) {
						commanded_state = button_state_to_latched_motor(tracked_press);
						motor_set_logged(commanded_state);
					} else {
						commanded_state = MOTOR_BRAKE;
						motor_set_logged(commanded_state);
					}
					tracked_press = BUTTON_NONE;
				} else if (button_state == BUTTON_BOTH) {
					tracked_press = BUTTON_BOTH;
					tracked_press_start_us = now_us;
					both_action_handled = false;
					stopping_latched_motion = false;
					commanded_state = MOTOR_BRAKE;
					motor_set_logged(commanded_state);
				} else {
					tracked_press = button_state;
					tracked_press_start_us = now_us;
					stopping_latched_motion = false;
					commanded_state = button_state_to_direct_motor(button_state);
					if (!(commanded_state == MOTOR_UP && motor_reed_stop_reached_if_due(commanded_state, &reed_closed_since_us))) {
						motor_set_logged(commanded_state);
					}
				}
			} else if (tracked_press == BUTTON_BOTH) {
				stopping_latched_motion = false;
				commanded_state = MOTOR_BRAKE;
				motor_set_logged(commanded_state);

				if (button_state == BUTTON_BOTH) {
					if (!both_action_handled && now_us - tracked_press_start_us >= 1000000LL) {
						both_action_handled = true;
						ota_mode(120000);
						stay_awake_until_us = esp_timer_get_time() + USER_INTERACTION_WINDOW_US;
					}
				} else {
					tracked_press = BUTTON_NONE;
					if (button_state == BUTTON_UP || button_state == BUTTON_DOWN) {
						tracked_press = button_state;
						tracked_press_start_us = esp_timer_get_time();
						stopping_latched_motion = false;
						commanded_state = button_state_to_direct_motor(button_state);
						if (!(commanded_state == MOTOR_UP && motor_reed_stop_reached_if_due(commanded_state, &reed_closed_since_us))) {
							motor_set_logged(commanded_state);
						}
					}
				}
			}

			if (motor_reed_stop_reached_if_due(commanded_state, &reed_closed_since_us)) {
				printf("[motor] reed stop reached\r\n");
				motor_set_logged(MOTOR_BRAKE);
				buzzer_buzz(2000, 200);
				if (tracked_press == BUTTON_UP || tracked_press == BUTTON_BOTH) {
					suppressed_buttons |= BUTTON_UP;
				}
				tracked_press = BUTTON_NONE;
				stopping_latched_motion = false;
				commanded_state = MOTOR_BRAKE;
			}
			if (motor_power_end_state_reached_if_due(commanded_state, &last_power_log_time_us, &low_power_since_us)) {
				motor_stop_after_power_end_state(commanded_state);
				suppressed_buttons |= raw_button_state;
				tracked_press = BUTTON_NONE;
				stopping_latched_motion = false;
				commanded_state = MOTOR_BRAKE;
			}

			vTaskDelay(pdMS_TO_TICKS(10));
		}

		motor_set_logged(MOTOR_OFF);
	} else { // WAKEUP_REASON_RTC or WAKEUP_REASON_UNKNOWN
		// Play tune if we woke up to to unknown reason
		if (wakeup_reason == WAKEUP_REASON_UNKNOWN) {
			buzzer_buzz(554, 100);
			buzzer_buzz(659, 100);
			buzzer_buzz(784, 100);
		}

		bool desired_state_reached = false;
		ESP_ERROR_CHECK(desired_door_state_is_reached(desired_state, &desired_state_reached));

		if (desired_state == DESIRED_DOOR_STATE_USER) {
			ESP_LOGI("main", "Desired door state is USER, skipping automatic reconciliation");
		} else if (desired_state_reached) {
			ESP_LOGI("main", "Desired door state %s is already reached", desired_door_state_to_string(desired_state));
		} else if (desired_state == DESIRED_DOOR_STATE_UP) {
			ESP_LOGI("main", "Desired door state UP not reached, moving up");
			motor_run_until_target(MOTOR_UP, CONFIG_MOTOR_RUNTIME_TIMEOUT, desired_state);
		} else if (desired_state == DESIRED_DOOR_STATE_DOWN) {
			ESP_LOGI("main", "Desired door state DOWN not reached, moving down");
			motor_run_until_target(MOTOR_DOWN, CONFIG_MOTOR_RUNTIME_TIMEOUT, desired_state);
		}

		printf("reporting to Sigfox\r\n");
		sigfox_report(true, false);
		printf("Sigfox report done\r\n");

		if (autoclose_is_due(autoclose_hours, time_hours, alarm_minutes, time_minutes)) {
			desired_state = DESIRED_DOOR_STATE_DOWN;
			ESP_ERROR_CHECK(desired_door_state_store(desired_state));
			disable_autoclose();
			ESP_LOGI("main", "Autoclose time matched after Sigfox report, storing desired door state DOWN");

			bool desired_state_down_reached = false;
			ESP_ERROR_CHECK(desired_door_state_is_reached(desired_state, &desired_state_down_reached));
			if (!desired_state_down_reached)
				motor_run_until_target(MOTOR_DOWN, CONFIG_MOTOR_RUNTIME_TIMEOUT, desired_state);
		}
	}

	ESP_ERROR_CHECK(ina219_powerdown());
	ESP_ERROR_CHECK(bme280adaptor_sleep());

	// Enter deep sleep
	printf("entering deep sleep\r\n");
	esp_err_t sleep_ready = input_wait_for_wakeup_inputs_idle(100, 2000);
	if (sleep_ready != ESP_OK)
		printf("Wake inputs still active before sleep: %s\r\n", esp_err_to_name(sleep_ready));
	ESP_ERROR_CHECK(esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL));
	ESP_ERROR_CHECK(input_configure_wakeup());
	esp_set_deep_sleep_wake_stub(esp_wake_deep_sleep);
	ESP_ERROR_CHECK(esp_deep_sleep_try_to_start());

	while(1);
}
