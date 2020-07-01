#ifndef _BATTERY_H
#define _BATTERY_H

esp_err_t battery_init(void);
float battery_get_voltage(void);

#endif
