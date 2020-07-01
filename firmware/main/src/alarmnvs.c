#include <nvs_flash.h>

esp_err_t alarmnvs_retrieve_time(uint8_t *minutes, uint8_t *hours) {
	nvs_handle_t nvshandle;

	esp_err_t res = nvs_open("chickendoor", NVS_READWRITE, &nvshandle);
	nvs_get_u8(nvshandle, "alarm_minutes", minutes);
	nvs_get_u8(nvshandle, "alarm_hours", hours);

	return res;
}

esp_err_t alarmnvs_store_time(uint8_t minutes, uint8_t hours) {
	nvs_handle_t nvshandle;

	esp_err_t res = nvs_open("chickendoor", NVS_READWRITE, &nvshandle);
	nvs_set_u8(nvshandle, "alarm_minutes", minutes);
	nvs_set_u8(nvshandle, "alarm_hours", hours);

	return res;
}

esp_err_t alarmnvs_retrieve_autoclose_hours(uint8_t *hours) {
	nvs_handle_t nvshandle;

	esp_err_t res = nvs_open("chickendoor", NVS_READWRITE, &nvshandle);
	nvs_get_u8(nvshandle, "autoclose_hours", hours);

	return res;
}

esp_err_t alarmnvs_store_autoclose_hours(uint8_t hours) {
	nvs_handle_t nvshandle;

	esp_err_t res = nvs_open("chickendoor", NVS_READWRITE, &nvshandle);
	nvs_set_u8(nvshandle, "autoclose_hours", hours);

	return res;
}

