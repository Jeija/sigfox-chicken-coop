#include <math.h>

#include "i2cdevice.h"
#include "ina219.h"

#define INA219_ADDR 0x40

#define INA219_REG_CONFIG 0x00
#define INA219_REG_SHUNT 0x01
#define INA219_REG_BUS 0x02
#define INA219_REG_POWER 0x03
#define INA219_REG_CURRENT 0x04
#define INA219_REG_CALIBRATION 0x05

#define INA219_SHUNT_RESISTANCE 0.1


static esp_err_t ina219_write_word(uint8_t reg_addr, uint16_t word)
{
	uint8_t data[2];
	data[0] = (word & 0xff00) >> 8;
	data[1] = (word & 0x00ff) >> 0;

	return i2cdevice_write(INA219_ADDR, reg_addr, data, 2);
}

static esp_err_t ina219_read_word(uint8_t reg_addr, uint16_t *word)
{
	uint8_t data[2];

	esp_err_t res = i2cdevice_read(INA219_ADDR, reg_addr, data, 2);
	*word = (data[0] << 8) | data[1];

	return res;
}

esp_err_t ina219_init(ina219_mode_t mode, ina219_fsr_t fsr, ina219_pga_t pga, ina219_adc_t busadc, ina219_adc_t shuntadc)
{
	uint16_t config = (fsr << 13) | (pga << 11) | (busadc << 7) | (shuntadc << 3) | mode;
	return ina219_write_word(INA219_REG_CONFIG, config);
}

esp_err_t ina219_bus_voltage(float *voltage)
{
	uint16_t busvoltage;

	esp_err_t res = ina219_read_word(INA219_REG_BUS, &busvoltage);
	*voltage = (busvoltage >> 3) * 0.004;

	return res;
}

esp_err_t ina219_shunt_voltage(float *voltage)
{
	int16_t shuntvoltage;

	esp_err_t res = ina219_read_word(INA219_REG_SHUNT, (uint16_t*) &shuntvoltage);
	*voltage = shuntvoltage * 0.00001;

	return res;
}

esp_err_t ina219_current(float *current)
{
	float shunt_voltage;

	esp_err_t res = ina219_shunt_voltage(&shunt_voltage);
	*current = fabsf(shunt_voltage) / INA219_SHUNT_RESISTANCE;

	return res;
}

esp_err_t ina219_power(float *power)
{
	float bus_voltage, current;

	esp_err_t res = ESP_OK;
	res |= ina219_bus_voltage(&bus_voltage);
	res |= ina219_current(&current);

	*power = bus_voltage * current;

	return res;
}

esp_err_t ina219_powerdown(void)
{
	return ina219_write_word(INA219_REG_CONFIG, 0x00);
}
