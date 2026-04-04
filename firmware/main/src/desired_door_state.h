#ifndef DESIRED_DOOR_STATE_H
#define DESIRED_DOOR_STATE_H

#include <stdbool.h>

#include <esp_err.h>

typedef enum {
	DESIRED_DOOR_STATE_USER = 0,
	DESIRED_DOOR_STATE_UP = 1,
	DESIRED_DOOR_STATE_DOWN = 2,
} desired_door_state_t;

esp_err_t desired_door_state_store(desired_door_state_t state);
esp_err_t desired_door_state_retrieve(desired_door_state_t *state);
esp_err_t desired_door_state_store_down_reached(bool reached);
esp_err_t desired_door_state_retrieve_down_reached(bool *reached);
const char *desired_door_state_to_string(desired_door_state_t state);

#endif
