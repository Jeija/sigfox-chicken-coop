#ifndef _MOTOR_H
#define _MOTOR_H

typedef enum {
	MOTOR_OFF = 0,
	MOTOR_UP = 1,
	MOTOR_DOWN = 2,
	MOTOR_BRAKE = 3
} motor_state_t;

esp_err_t motor_init(void);
esp_err_t motor_set(motor_state_t state);

#endif
