#ifndef _I2CDEVICE_H
#define _I2CDEVICE_H

#define I2C_NUM I2C_NUM_0
#define I2C_SPEED 100000
#define I2C_ACK_CHECK_EN 1

esp_err_t i2cdevice_init(void);

esp_err_t i2cdevice_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
esp_err_t i2cdevice_write_byte(uint8_t dev_addr, uint8_t reg_add, uint8_t data);
esp_err_t i2cdevice_write_bit(uint8_t dev_addr, uint8_t reg_add, uint8_t bit, uint8_t val);

esp_err_t i2cdevice_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
esp_err_t i2cdevice_read_byte(uint8_t dev_addr, uint8_t reg_add, uint8_t *data);

#endif
