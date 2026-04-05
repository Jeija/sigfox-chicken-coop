#ifndef _MOTOR_H
#define _MOTOR_H

typedef enum {
	MOTOR_OFF = 0,
	MOTOR_UP = 1,
	MOTOR_DOWN = 2,
	MOTOR_BRAKE = 3,
	MOTOR_UP_UNTIL_LATCH = 4,
	MOTOR_DOWN_UNTIL_LATCH = 5
} motor_state_t;

esp_err_t motor_init(void);
esp_err_t motor_set(motor_state_t state);
motor_state_t motor_get_state(void);
const char *motor_state_to_string(motor_state_t state);

#endif
