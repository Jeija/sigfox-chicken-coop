#include <stddef.h>

#include <driver/i2c_master.h>

#include "i2cdevice.h"

// The project uses a small, fixed set of I2C devices; keep handles cached so
// the rest of the code can preserve the old register-based helper API.
#define I2C_PORT I2C_NUM_0
#define I2C_SPEED 100000
#define I2C_TIMEOUT_MS 1000
#define I2C_MAX_DEVICES 8

typedef struct {
	uint8_t address;
	i2c_master_dev_handle_t handle;
} i2cdevice_entry_t;

static i2c_master_bus_handle_t i2c_bus;
static i2cdevice_entry_t i2c_devices[I2C_MAX_DEVICES];
static size_t i2c_device_count = 0;

static esp_err_t i2cdevice_get_handle(uint8_t dev_addr, i2c_master_dev_handle_t *out_handle)
{
	for (size_t i = 0; i < i2c_device_count; ++i) {
		if (i2c_devices[i].address == dev_addr) {
			*out_handle = i2c_devices[i].handle;
			return ESP_OK;
		}
	}

	if (i2c_device_count >= I2C_MAX_DEVICES)
		return ESP_ERR_NO_MEM;

	i2c_device_config_t dev_config = {
		.dev_addr_length = I2C_ADDR_BIT_LEN_7,
		.device_address = dev_addr,
		.scl_speed_hz = I2C_SPEED,
		.scl_wait_us = 0,
	};
	esp_err_t res = i2c_master_bus_add_device(i2c_bus, &dev_config, &i2c_devices[i2c_device_count].handle);
	if (res != ESP_OK)
		return res;

	i2c_devices[i2c_device_count].address = dev_addr;
	*out_handle = i2c_devices[i2c_device_count].handle;
	++i2c_device_count;
	return ESP_OK;
}

esp_err_t i2cdevice_init(void)
{
	if (i2c_bus != NULL)
		return ESP_OK;

	i2c_master_bus_config_t i2c_config = {
		.i2c_port = I2C_PORT,
		.sda_io_num = CONFIG_GPIO_I2C_SDA,
		.scl_io_num = CONFIG_GPIO_I2C_SCL,
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.glitch_ignore_cnt = 7,
		.intr_priority = 0,
		.trans_queue_depth = 0,
		.flags.enable_internal_pullup = true,
		.flags.allow_pd = false,
	};

	return i2c_new_master_bus(&i2c_config, &i2c_bus);
}

esp_err_t i2cdevice_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	i2c_master_dev_handle_t dev_handle;
	esp_err_t res = i2cdevice_get_handle(dev_addr, &dev_handle);
	if (res != ESP_OK)
		return res;

	i2c_master_transmit_multi_buffer_info_t buffers[] = {
		{
			.write_buffer = &reg_addr,
			.buffer_size = 1,
		},
		{
			.write_buffer = reg_data,
			.buffer_size = len,
		},
	};

	return i2c_master_multi_buffer_transmit(dev_handle, buffers, 2, I2C_TIMEOUT_MS);
}

esp_err_t i2cdevice_write_byte(uint8_t dev_addr, uint8_t reg_add, uint8_t data)
{
	return i2cdevice_write(dev_addr, reg_add, &data, 1);
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
	i2c_master_dev_handle_t dev_handle;
	esp_err_t res = i2cdevice_get_handle(dev_addr, &dev_handle);
	if (res != ESP_OK)
		return res;

	return i2c_master_transmit_receive(dev_handle, &reg_addr, 1, reg_data, len, I2C_TIMEOUT_MS);
}

esp_err_t i2cdevice_read_byte(uint8_t dev_addr, uint8_t reg_add, uint8_t *data)
{
	return i2cdevice_read(dev_addr, reg_add, data, 1);
}
