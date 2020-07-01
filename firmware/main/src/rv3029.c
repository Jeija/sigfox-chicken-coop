#include <freertos/FreeRTOS.h>

#include "i2cdevice.h"

#define RV3029_ADDR 0x56

#define BCD2BIN(val) ((((val) & 0x0f) + ((val) >> 4) * 10))
#define BIN2BCD(val) ((((val)/10)<<4) + (val)%10)

esp_err_t rv3029_init()
{
	esp_err_t res = ESP_OK;

	/* Reset RV3029 */
	res |= i2cdevice_write_byte(RV3029_ADDR, 0x04, 0x10);
	res |= i2cdevice_write_bit(RV3029_ADDR, 0x03, 4, 0);

	/* Enable alarm interrupt and clear interrupt flags */
	res |= i2cdevice_write_byte(RV3029_ADDR, 0x01, 0x01);
	res |= i2cdevice_write_byte(RV3029_ADDR, 0x02, 0x00);

	return res;
}

/* alarm occurs every hour - this only configures the *minute* at which it will occur */
esp_err_t rv3029_set_alarm_minutes(uint8_t minutes)
{
	esp_err_t res = ESP_OK;

	res |= i2cdevice_write_byte(RV3029_ADDR, 0x10, (1 << 7) | BIN2BCD(0));
	res |= i2cdevice_write_byte(RV3029_ADDR, 0x11, (1 << 7) | BIN2BCD(minutes));
	res |= i2cdevice_write_byte(RV3029_ADDR, 0x12, 0x00);
	res |= i2cdevice_write_byte(RV3029_ADDR, 0x13, 0x00);
	res |= i2cdevice_write_byte(RV3029_ADDR, 0x14, 0x00);
	res |= i2cdevice_write_byte(RV3029_ADDR, 0x15, 0x00);
	res |= i2cdevice_write_byte(RV3029_ADDR, 0x16, 0x00);

	return res;
}

esp_err_t rv3029_temp(int8_t *temp)
{
	esp_err_t res = i2cdevice_read_byte(RV3029_ADDR, 0x20, (uint8_t *) temp);
	temp = temp - 60;

	return res;
}

esp_err_t rv3029_gettime(uint8_t *seconds, uint8_t *minutes, uint8_t *hours)
{
	uint8_t raw[3];
	esp_err_t res = i2cdevice_read(RV3029_ADDR, 0x08, (uint8_t*) raw, 3);

	if (seconds != NULL)
		*seconds = BCD2BIN(raw[0]);

	if (minutes != NULL)
		*minutes = BCD2BIN(raw[1]);

	if (hours != NULL)
		*hours = BCD2BIN(raw[2]);

	return res;
}

esp_err_t rv3029_settime(uint8_t seconds, uint8_t minutes, uint8_t hours)
{
	uint8_t raw[3];
	raw[0] = BIN2BCD(seconds);
	raw[1] = BIN2BCD(minutes);
	raw[2] = BIN2BCD(hours);

	return i2cdevice_write(RV3029_ADDR, 0x08, (uint8_t*) raw, 3);
}
