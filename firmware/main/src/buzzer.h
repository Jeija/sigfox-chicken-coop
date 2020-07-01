#ifndef _BUZZER_H
#define _BUZZER_H

esp_err_t buzzer_init(void);
esp_err_t buzzer_buzz(int freq, int duration);

#endif
