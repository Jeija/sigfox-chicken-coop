#include <nvs.h>

#include "desired_door_state.h"

#define DESIRED_DOOR_STATE_NAMESPACE "chickendoor"
#define DESIRED_DOOR_STATE_KEY "desired_state"
#define DESIRED_DOOR_DOWN_KEY "door_down_ok"

static esp_err_t desired_door_state_open(nvs_handle_t *handle)
{
	return nvs_open(DESIRED_DOOR_STATE_NAMESPACE, NVS_READWRITE, handle);
}

esp_err_t desired_door_state_store(desired_door_state_t state)
{
	nvs_handle_t handle;
	esp_err_t res = desired_door_state_open(&handle);
	if (res != ESP_OK)
		return res;

	res = nvs_set_u8(handle, DESIRED_DOOR_STATE_KEY, (uint8_t) state);
	if (res == ESP_OK)
		res = nvs_commit(handle);

	nvs_close(handle);
	return res;
}

esp_err_t desired_door_state_retrieve(desired_door_state_t *state)
{
	nvs_handle_t handle;
	esp_err_t res = desired_door_state_open(&handle);
	if (res != ESP_OK)
		return res;

	uint8_t raw_state = DESIRED_DOOR_STATE_USER;
	res = nvs_get_u8(handle, DESIRED_DOOR_STATE_KEY, &raw_state);
	nvs_close(handle);

	if (res == ESP_ERR_NVS_NOT_FOUND) {
		*state = DESIRED_DOOR_STATE_USER;
		return ESP_OK;
	}

	if (res != ESP_OK)
		return res;

	if (raw_state > DESIRED_DOOR_STATE_DOWN)
		raw_state = DESIRED_DOOR_STATE_USER;

	*state = (desired_door_state_t) raw_state;
	return ESP_OK;
}

esp_err_t desired_door_state_store_down_reached(bool reached)
{
	nvs_handle_t handle;
	esp_err_t res = desired_door_state_open(&handle);
	if (res != ESP_OK)
		return res;

	res = nvs_set_u8(handle, DESIRED_DOOR_DOWN_KEY, reached ? 1 : 0);
	if (res == ESP_OK)
		res = nvs_commit(handle);

	nvs_close(handle);
	return res;
}

esp_err_t desired_door_state_retrieve_down_reached(bool *reached)
{
	nvs_handle_t handle;
	esp_err_t res = desired_door_state_open(&handle);
	if (res != ESP_OK)
		return res;

	uint8_t raw_reached = 0;
	res = nvs_get_u8(handle, DESIRED_DOOR_DOWN_KEY, &raw_reached);
	nvs_close(handle);

	if (res == ESP_ERR_NVS_NOT_FOUND) {
		*reached = false;
		return ESP_OK;
	}

	if (res != ESP_OK)
		return res;

	*reached = raw_reached != 0;
	return ESP_OK;
}

const char *desired_door_state_to_string(desired_door_state_t state)
{
	switch (state) {
		case DESIRED_DOOR_STATE_UP:
			return "UP";
		case DESIRED_DOOR_STATE_DOWN:
			return "DOWN";
		case DESIRED_DOOR_STATE_USER:
		default:
			return "USER";
	}
}
