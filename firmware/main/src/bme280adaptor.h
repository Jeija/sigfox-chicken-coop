#include <freertos/FreeRTOS.h>

#ifndef _BME280DEVICE_H
#define _BME280DEVICE_H

esp_err_t bme280adaptor_init(void);
esp_err_t bme280adaptor_get_data(double *temperature, double *pressure, double *humidity);
esp_err_t bme280adaptor_sleep(void);

#endif
