#include <driver/gpio.h>

#include "motor.h"

esp_err_t motor_init(void)
{
	esp_err_t res = ESP_OK;

	gpio_pad_select_gpio(CONFIG_GPIO_MOTOR_UP);
	res |= gpio_set_direction(CONFIG_GPIO_MOTOR_UP, GPIO_MODE_OUTPUT);

	gpio_pad_select_gpio(CONFIG_GPIO_MOTOR_DOWN);
	res |= gpio_set_direction(CONFIG_GPIO_MOTOR_DOWN, GPIO_MODE_OUTPUT);

	return res;
}

esp_err_t motor_set(motor_state_t state)
{
	esp_err_t res = ESP_OK;

	res |= gpio_set_level(CONFIG_GPIO_MOTOR_UP, (state == MOTOR_UP || state == MOTOR_BRAKE) ? 1 : 0);
	res |= gpio_set_level(CONFIG_GPIO_MOTOR_DOWN, (state == MOTOR_DOWN || state == MOTOR_BRAKE) ? 1 : 0);

	return res;
}
