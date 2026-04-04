#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <driver/rtc_io.h>
#include <driver/gpio.h>
#include <esp_sleep.h>

#include "input.h"
#include "motor.h"

#define ALL_INPUTS_MASK (1ull << CONFIG_GPIO_RTC_INT) | (1ull << CONFIG_GPIO_BUTTON_UP) | (1ull << CONFIG_GPIO_BUTTON_DOWN)

static button_state_t last_button_state = BUTTON_NONE;

extern motor_state_t wake_stub_motor_request;

esp_err_t input_init(void)
{
	esp_err_t ret = ESP_OK;

	ret |= rtc_gpio_deinit(CONFIG_GPIO_RTC_INT);
	ret |= rtc_gpio_deinit(CONFIG_GPIO_BUTTON_UP);
	ret |= rtc_gpio_deinit(CONFIG_GPIO_BUTTON_DOWN);
	ret |= rtc_gpio_deinit(CONFIG_GPIO_REED);

	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.pin_bit_mask = ALL_INPUTS_MASK | (1ull << CONFIG_GPIO_REED);
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	ret |= gpio_config(&io_conf);
	last_button_state = BUTTON_NONE;

	return ret;
}

esp_err_t input_configure_wakeup(void)
{
	return esp_sleep_enable_ext1_wakeup(ALL_INPUTS_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);
}

esp_err_t input_wait_for_wakeup_inputs_idle(uint32_t debounce_ms, uint32_t timeout_ms)
{
	uint32_t idle_ms = 0;
	uint32_t waited_ms = 0;
	const uint32_t poll_ms = 10;

	while (idle_ms < debounce_ms) {
		bool any_active = gpio_get_level(CONFIG_GPIO_RTC_INT) ||
				gpio_get_level(CONFIG_GPIO_BUTTON_UP) ||
				gpio_get_level(CONFIG_GPIO_BUTTON_DOWN);

		if (!any_active) {
			idle_ms += poll_ms;
		} else {
			idle_ms = 0;
		}

		if (waited_ms >= timeout_ms)
			return ESP_ERR_TIMEOUT;

		vTaskDelay(pdMS_TO_TICKS(poll_ms));
		waited_ms += poll_ms;
	}

	return ESP_OK;
}

wakeup_reason_t input_get_wakeup_reason(void)
{
	esp_sleep_wakeup_cause_t wakeup_causes = esp_sleep_get_wakeup_causes();

	if ((wakeup_causes & BIT(ESP_SLEEP_WAKEUP_EXT1)) != 0) {
		uint64_t wakeup_mask = esp_sleep_get_ext1_wakeup_status();
		bool rtc_woke = (wakeup_mask & (1ull << CONFIG_GPIO_RTC_INT)) != 0;
		bool button_woke = (wakeup_mask & ((1ull << CONFIG_GPIO_BUTTON_UP) | (1ull << CONFIG_GPIO_BUTTON_DOWN))) != 0;
		printf("[wakeup] ext1 wake mask: 0x%llx\r\n", wakeup_mask);

		if (rtc_woke && !button_woke) {
			printf("[wakeup] ext1 wake classified as RTC only\r\n");
			return WAKEUP_REASON_RTC;
		}

		if (button_woke && !rtc_woke) {
			printf("[wakeup] ext1 wake classified as USER only\r\n");
			return WAKEUP_REASON_USER;
		}

		if (rtc_woke && button_woke) {
			if (wake_stub_motor_request != MOTOR_OFF) {
				printf("[wakeup] ext1 wake had RTC+button bits, wake stub confirms USER wake\r\n");
				return WAKEUP_REASON_USER;
			}

			if (gpio_get_level(CONFIG_GPIO_BUTTON_UP) || gpio_get_level(CONFIG_GPIO_BUTTON_DOWN)) {
				printf("[wakeup] ext1 wake had RTC+button bits, live button level confirms USER wake\r\n");
				return WAKEUP_REASON_USER;
			}

			printf("[wakeup] ext1 wake had RTC+button bits, classifying as RTC\r\n");
			return WAKEUP_REASON_RTC;
		}
	}

	// Fall back to live levels for cold boots or resets where no deep-sleep wake
	// cause is available but a button is already held.
	if (gpio_get_level(CONFIG_GPIO_BUTTON_UP) || gpio_get_level(CONFIG_GPIO_BUTTON_DOWN)) {
		printf("[wakeup] fallback live button level caused USER wake\r\n");
		return WAKEUP_REASON_USER;
	}

	if (gpio_get_level(CONFIG_GPIO_RTC_INT)) {
		printf("[wakeup] fallback live RTC interrupt level caused RTC wake\r\n");
		return WAKEUP_REASON_RTC;
	}

	return WAKEUP_REASON_UNKNOWN;
}

button_state_t input_get_button_state(void)
{
	button_state_t button_state = (gpio_get_level(CONFIG_GPIO_BUTTON_UP) << 1) | gpio_get_level(CONFIG_GPIO_BUTTON_DOWN);

	if (((last_button_state ^ button_state) & BUTTON_UP) != 0)
		printf("[input] up button %s\r\n", (button_state & BUTTON_UP) ? "pressed" : "released");

	if (((last_button_state ^ button_state) & BUTTON_DOWN) != 0)
		printf("[input] down button %s\r\n", (button_state & BUTTON_DOWN) ? "pressed" : "released");

	last_button_state = button_state;
	return button_state;
}

bool input_get_reed_state(void)
{
	return gpio_get_level(CONFIG_GPIO_REED);
}
