#ifndef _SOLAR_H
#define _SOLAR_H

esp_err_t solar_init(void);

bool solar_is_charging(void);
bool solar_is_fault(void);

#endif
