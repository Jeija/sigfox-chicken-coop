#include <driver/gpio.h>

esp_err_t solar_init(void)
{
	esp_err_t res = ESP_OK;

	gpio_pad_select_gpio(CONFIG_GPIO_SOLAR_CHARGE);
	res |= gpio_set_direction(CONFIG_GPIO_SOLAR_CHARGE, GPIO_MODE_INPUT);

	gpio_pad_select_gpio(CONFIG_GPIO_SOLAR_FAULT);
	res |= gpio_set_direction(CONFIG_GPIO_SOLAR_FAULT, GPIO_MODE_INPUT);

	return res;
}

bool solar_is_charging(void)
{
	return !gpio_get_level(CONFIG_GPIO_SOLAR_CHARGE);
}

bool solar_is_fault(void)
{
	return !gpio_get_level(CONFIG_GPIO_SOLAR_FAULT);
}
