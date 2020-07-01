#ifndef _SIGFOX_H
#define _SIGFOX_H

esp_err_t sigfox_init(void);
esp_err_t sigfox_report(bool request_downlink, bool is_ack);

#endif
