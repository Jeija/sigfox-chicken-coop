#ifndef _INPUT_H
#define _INPUT_H

typedef enum {
	WAKEUP_REASON_USER = 0,
	WAKEUP_REASON_RTC = 1,
	WAKEUP_REASON_UNKNOWN = 2
} wakeup_reason_t;

typedef enum {
	BUTTON_NONE = 0b00,
	BUTTON_DOWN = 0b01,
	BUTTON_UP = 0b10,
	BUTTON_BOTH = 0b11
} button_state_t;

esp_err_t input_init(void);
esp_err_t input_configure_wakeup(void);
wakeup_reason_t input_get_wakeup_reason(void);
button_state_t input_get_button_state(void);
bool input_get_reed_state(void);

#endif
