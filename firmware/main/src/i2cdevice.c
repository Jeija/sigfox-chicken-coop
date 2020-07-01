#include <driver/i2c.h>

#include "i2cdevice.h"

esp_err_t i2cdevice_init(void)
{
	esp_err_t res = ESP_OK;

	i2c_config_t i2c_config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = CONFIG_GPIO_I2C_SDA,
		.sda_pullup_en = true,
		.scl_io_num = CONFIG_GPIO_I2C_SCL,
		.scl_pullup_en = true,
		.master.clk_speed = I2C_SPEED
	};

	res |= i2c_param_config(I2C_NUM, &i2c_config);
	res |= i2c_driver_install(I2C_NUM, i2c_config.mode, 0, 0, 0);

	return res;
}

esp_err_t i2cdevice_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	esp_err_t res = ESP_OK;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	res |= i2c_master_start(cmd);
	res |= i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, I2C_ACK_CHECK_EN);
	res |= i2c_master_write_byte(cmd, reg_addr, I2C_ACK_CHECK_EN);
	res |= i2c_master_write(cmd, reg_data, len, I2C_ACK_CHECK_EN);
	res |= i2c_master_stop(cmd);

	res |= i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	return res;
}

esp_err_t i2cdevice_write_byte(uint8_t dev_addr, uint8_t reg_add, uint8_t data)
{
	esp_err_t res = ESP_OK;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	res |= i2c_master_start(cmd);
	res |= i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, I2C_ACK_CHECK_EN);
	res |= i2c_master_write_byte(cmd, reg_add, I2C_ACK_CHECK_EN);
	res |= i2c_master_write_byte(cmd, data, I2C_ACK_CHECK_EN);
	res |= i2c_master_stop(cmd);

	res |= i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	return res;
}

esp_err_t i2cdevice_write_bit(uint8_t dev_addr, uint8_t reg_add, uint8_t bit, uint8_t val)
{
	esp_err_t res = ESP_OK;

	uint8_t data = 0x00;
	res |= i2cdevice_read_byte(dev_addr, reg_add, &data);

	if (bit)
		data |= 1 << bit;
	else
		data &= ~(1 << bit);

	res |= i2cdevice_write_byte(dev_addr, reg_add, data);

	return res;
}

esp_err_t i2cdevice_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	esp_err_t res = ESP_OK;

	// write address to read from
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	res |= i2c_master_start(cmd);
	res |= i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, I2C_ACK_CHECK_EN);
	res |= i2c_master_write_byte(cmd, reg_addr, I2C_ACK_CHECK_EN);
	res |= i2c_master_stop(cmd);
	res |= i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	// read from address
	cmd = i2c_cmd_link_create();
	res |= i2c_master_start(cmd);
	res |= i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, I2C_ACK_CHECK_EN);
	res |= i2c_master_read(cmd, reg_data, len, I2C_MASTER_LAST_NACK);
	res |= i2c_master_stop(cmd);
	res |= i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	return res;
}

esp_err_t i2cdevice_read_byte(uint8_t dev_addr, uint8_t reg_add, uint8_t *data)
{
	esp_err_t res = ESP_OK;

	// write address to read from
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	res |= i2c_master_start(cmd);
	res |= i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, I2C_ACK_CHECK_EN);
	res |= i2c_master_write_byte(cmd, reg_add, I2C_ACK_CHECK_EN);
	res |= i2c_master_stop(cmd);
	res |= i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	// read from address
	cmd = i2c_cmd_link_create();
	res |= i2c_master_start(cmd);
	res |= i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, I2C_ACK_CHECK_EN);
	res |= i2c_master_read_byte(cmd, data, I2C_MASTER_NACK);
	res |= i2c_master_stop(cmd);
	res |= i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	return res;
}
