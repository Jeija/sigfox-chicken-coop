#ifndef _ALARMNVS_H
#define _ALARMNVS_H

esp_err_t alarmnvs_retrieve_time(uint8_t *minutes, uint8_t *hours);
esp_err_t alarmnvs_store_time(uint8_t minutes, uint8_t hours);

esp_err_t alarmnvs_retrieve_autoclose_hours(uint8_t *hours);
esp_err_t alarmnvs_store_autoclose_hours(uint8_t hours);


#endif
