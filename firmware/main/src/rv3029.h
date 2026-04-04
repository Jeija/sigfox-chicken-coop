#ifndef RV3029_H
#define RV3029_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

esp_err_t rv3029_init();

esp_err_t rv3029_set_testing_mode(bool enabled);
esp_err_t rv3029_set_alarm_minutes(uint8_t minutes);
esp_err_t rv3029_gettime(uint8_t *seconds, uint8_t *minutes, uint8_t *hours);
esp_err_t rv3029_settime(uint8_t seconds, uint8_t minutes, uint8_t hours);
esp_err_t rv3029_temp(int8_t *temp);

#endif
