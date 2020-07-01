#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "i2cdevice.h"
#include "bme280_defs.h"
#include "bme280.h"

#define I2C_NUM I2C_NUM_0
#define ACK_CHECK_EN 1

static struct bme280_dev dev;
static bool bme280_initialized = false;

void bme280adaptor_wait_ms(uint32_t period)
{
	vTaskDelay(period / portTICK_PERIOD_MS);
}

int8_t bme280adaptor_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	return i2cdevice_write(dev_addr, reg_addr, reg_data, len) == ESP_OK ? 0 : 1;
}

int8_t bme280adaptor_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	return i2cdevice_read(dev_addr, reg_addr, reg_data, len) == ESP_OK ? 0 : 1;
}

esp_err_t bme280adaptor_init(void)
{
	int res = BME280_OK;

	dev.dev_id = BME280_I2C_ADDR_PRIM;
	dev.intf = BME280_I2C_INTF;
	dev.read = bme280adaptor_read;
	dev.write = bme280adaptor_write;
	dev.delay_ms = bme280adaptor_wait_ms;
	res |= bme280_init(&dev);

	dev.settings.osr_h = BME280_OVERSAMPLING_1X;
	dev.settings.osr_p = BME280_OVERSAMPLING_16X;
	dev.settings.osr_t = BME280_OVERSAMPLING_2X;
	dev.settings.filter = BME280_FILTER_COEFF_16;
	dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	uint8_t settings_sel;
	settings_sel = BME280_OSR_PRESS_SEL;
	settings_sel |= BME280_OSR_TEMP_SEL;
	settings_sel |= BME280_OSR_HUM_SEL;
	settings_sel |= BME280_STANDBY_SEL;
	settings_sel |= BME280_FILTER_SEL;
	res |= bme280_set_sensor_settings(settings_sel, &dev);
	res |= bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev);

	bme280_initialized = (res == BME280_OK);

	return res == BME280_OK ? ESP_OK : ESP_FAIL;
}

esp_err_t bme280adaptor_get_data(double *temperature, double *pressure, double *humidity)
{
	struct bme280_data comp_data;

	int8_t res = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
	*temperature = comp_data.temperature;
	*pressure = comp_data.pressure;
	*humidity = comp_data.humidity;

	return res == BME280_OK ? ESP_OK : ESP_FAIL;
}

esp_err_t bme280adaptor_sleep(void)
{
	if (bme280_initialized)
		return bme280_set_sensor_mode(BME280_SLEEP_MODE, &dev) == BME280_OK ? ESP_OK : ESP_FAIL;

	return ESP_OK;
}
