#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/rtc_io.h>
#include <driver/gpio.h>
#include <esp_sleep.h>

#include "input.h"

#define ALL_INPUTS_MASK (1ull << CONFIG_GPIO_RTC_INT) | (1ull << CONFIG_GPIO_BUTTON_UP) | (1ull << CONFIG_GPIO_BUTTON_DOWN)

esp_err_t input_init(void)
{
	esp_err_t ret = ESP_OK;

	ret |= rtc_gpio_deinit(CONFIG_GPIO_RTC_INT);
	ret |= rtc_gpio_deinit(CONFIG_GPIO_BUTTON_UP);
	ret |= rtc_gpio_deinit(CONFIG_GPIO_BUTTON_DOWN);
	ret |= rtc_gpio_deinit(CONFIG_GPIO_REED);

	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.pin_bit_mask = ALL_INPUTS_MASK | (1ull << CONFIG_GPIO_REED);
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	ret |= gpio_config(&io_conf);

	return ret;
}

esp_err_t input_configure_wakeup(void)
{
	return esp_sleep_enable_ext1_wakeup(ALL_INPUTS_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);
}

wakeup_reason_t input_get_wakeup_reason(void)
{
	// Check input level a couple of times (sort-of debouncing)
	for (uint8_t i = 0; i < 10; ++i) {
		if (gpio_get_level(CONFIG_GPIO_BUTTON_UP) || gpio_get_level(CONFIG_GPIO_BUTTON_DOWN)) {
			return WAKEUP_REASON_USER;
		}

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}

	if (gpio_get_level(CONFIG_GPIO_RTC_INT)) {
		return WAKEUP_REASON_RTC;
	}

	return WAKEUP_REASON_UNKNOWN;
}

button_state_t input_get_button_state(void)
{
	return (gpio_get_level(CONFIG_GPIO_BUTTON_UP) << 1) | gpio_get_level(CONFIG_GPIO_BUTTON_DOWN);
}

bool input_get_reed_state(void)
{
	return gpio_get_level(CONFIG_GPIO_REED);
}
