#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/rtc_io.h>
#include <esp_sleep.h>
#include <sdkconfig.h>
#include <nvs_flash.h>

#include "bme280adaptor.h"
#include "i2cdevice.h"
#include "alarmnvs.h"
#include "battery.h"
#include "sigfox.h"
#include "rv3029.h"
#include "ina219.h"
#include "buzzer.h"
#include "motor.h"
#include "solar.h"
#include "input.h"
#include "ota.h"

#include "renard_phy_s2lp_protocol.h"
#include "renard_phy_s2lp.h"

esp_err_t nvs_init(void)
{
	esp_err_t ret = nvs_flash_init();

	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}

	return ret;
}

/* start motor *immediately* upon button press */
void RTC_IRAM_ATTR esp_wake_deep_sleep(void) {
	esp_default_wake_deep_sleep();

	gpio_matrix_out(CONFIG_GPIO_MOTOR_UP, SIG_GPIO_OUT_IDX, false, false);
	gpio_matrix_out(CONFIG_GPIO_MOTOR_DOWN, SIG_GPIO_OUT_IDX, false, false);

	if (!GPIO_INPUT_GET(CONFIG_GPIO_REED))
		GPIO_OUTPUT_SET(CONFIG_GPIO_MOTOR_UP, GPIO_INPUT_GET(CONFIG_GPIO_BUTTON_UP));
	GPIO_OUTPUT_SET(CONFIG_GPIO_MOTOR_DOWN, GPIO_INPUT_GET(CONFIG_GPIO_BUTTON_DOWN));
}

void app_main(void)
{
	// Determine wakeup reason
	ESP_ERROR_CHECK(input_init());
	wakeup_reason_t wakeup_reason = input_get_wakeup_reason();

	if (wakeup_reason != WAKEUP_REASON_UNKNOWN) {
		// Initialize all systems
		ESP_ERROR_CHECK(ota_init());
		ESP_ERROR_CHECK(nvs_init());
		ESP_ERROR_CHECK(i2cdevice_init());

		ESP_ERROR_CHECK(bme280adaptor_init());
		ESP_ERROR_CHECK(ina219_init(INA219_MODE_CONT_SH_BUS, INA219_FSR_32V, INA219_PGA_80MV, INA219_ADC_12BIT, INA219_ADC_12BIT));
		ESP_ERROR_CHECK(rv3029_init());
		ESP_ERROR_CHECK(buzzer_init());
		ESP_ERROR_CHECK(motor_init());
		ESP_ERROR_CHECK(battery_init());
		ESP_ERROR_CHECK(solar_init());
		ESP_ERROR_CHECK(sigfox_init());

		uint8_t alarm_minutes, alarm_hours, autoclose_hours;
		ESP_ERROR_CHECK(alarmnvs_retrieve_time(&alarm_minutes, &alarm_hours));
		ESP_ERROR_CHECK(alarmnvs_retrieve_autoclose_hours(&autoclose_hours));
		ESP_ERROR_CHECK(rv3029_set_alarm_minutes(alarm_minutes));

		uint8_t time_minutes, time_hours;
		ESP_ERROR_CHECK(rv3029_gettime(NULL, &time_minutes, &time_hours));

		// some devices (pressure sensor, INA219) need some time to initialize...
		vTaskDelay(200 / portTICK_PERIOD_MS);

		// Use pressed button: drive motor
		if (wakeup_reason == WAKEUP_REASON_USER) {
			button_state_t button_state;
			bool stall = false;
			ina219_stall_reset();

			while ((button_state = input_get_button_state()) != BUTTON_NONE) {
				if (button_state == BUTTON_DOWN) {
					motor_set(MOTOR_DOWN);
				} else if (button_state == BUTTON_UP && !stall && !input_get_reed_state()) {
					motor_set(MOTOR_UP);
					ESP_ERROR_CHECK(ina219_stall_detect(&stall));
				} else {
					motor_set(MOTOR_BRAKE);
					ina219_stall_reset();

					if (button_state == BUTTON_BOTH) {
						vTaskDelay(1000 / portTICK_PERIOD_MS);
						if (input_get_button_state() == BUTTON_BOTH) {
							ota_mode(120000);
						} else if (input_get_button_state() == BUTTON_NONE) {
							// Perform sigfox report, play chirp
							vTaskDelay(100 / portTICK_PERIOD_MS);
							buzzer_buzz(554, 100);
							buzzer_buzz(659, 100);
							buzzer_buzz(784, 100);
							vTaskDelay(100 / portTICK_PERIOD_MS);

							sigfox_report(true, false);
						}
					}
				}
				vTaskDelay(10 / portTICK_PERIOD_MS);
			}

			motor_set(MOTOR_OFF);
		} else if (wakeup_reason == WAKEUP_REASON_RTC) {
			// auto-open
			if (alarm_hours == time_hours && alarm_minutes == time_minutes) {
				motor_set(MOTOR_UP);
				ina219_stall_reset();
				bool stall = false;
				int64_t start_time = esp_timer_get_time();
				while (!input_get_reed_state() && !stall && (esp_timer_get_time() - start_time) / 1000000 < CONFIG_ALARM_MOTOR_UPTIME_MAX) {
					ESP_ERROR_CHECK(ina219_stall_detect(&stall));
					vTaskDelay(10 / portTICK_PERIOD_MS);
				}
				vTaskDelay(50 / portTICK_PERIOD_MS);
				motor_set(MOTOR_BRAKE);
			}

			// auto-close - only do this once, then disable it
			if (autoclose_hours == time_hours && alarm_minutes == time_minutes) {
				motor_set(MOTOR_DOWN);
				int64_t start_time = esp_timer_get_time();
				while ((esp_timer_get_time() - start_time) / 1000000 < CONFIG_AUTOCLOSE_MOTOR_DOWNTIME) {
					vTaskDelay(10 / portTICK_PERIOD_MS);
				}
				alarmnvs_store_autoclose_hours(25);
				motor_set(MOTOR_BRAKE);
			}

			sigfox_report(true, false);
		}

		ESP_ERROR_CHECK(ina219_powerdown());
		ESP_ERROR_CHECK(bme280adaptor_sleep());
	}

	// Enter deep sleep
	printf("entering deep sleep\r\n");
	ESP_ERROR_CHECK(esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL));
	ESP_ERROR_CHECK(input_configure_wakeup());

    esp_restart();
	esp_deep_sleep_start();

	while(1);
}
