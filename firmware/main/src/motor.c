#include <driver/gpio.h>

#include "motor.h"

esp_err_t motor_init(void)
{
	esp_err_t res = ESP_OK;

	res |= gpio_reset_pin(CONFIG_GPIO_MOTOR_UP);
	res |= gpio_set_direction(CONFIG_GPIO_MOTOR_UP, GPIO_MODE_OUTPUT);

	res |= gpio_reset_pin(CONFIG_GPIO_MOTOR_DOWN);
	res |= gpio_set_direction(CONFIG_GPIO_MOTOR_DOWN, GPIO_MODE_OUTPUT);

	return res;
}

esp_err_t motor_set(motor_state_t state)
{
	esp_err_t res = ESP_OK;

	res |= gpio_set_level(CONFIG_GPIO_MOTOR_UP, (state == MOTOR_UP || state == MOTOR_UP_UNTIL_LATCH || state == MOTOR_BRAKE) ? 1 : 0);
	res |= gpio_set_level(CONFIG_GPIO_MOTOR_DOWN, (state == MOTOR_DOWN || state == MOTOR_DOWN_UNTIL_LATCH || state == MOTOR_BRAKE) ? 1 : 0);

	return res;
}
