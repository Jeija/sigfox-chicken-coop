#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_check.h>
#include <esp_err.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_system.h>

#include "alarmnvs.h"
#include "battery.h"
#include "bme280adaptor.h"
#include "desired_door_state.h"
#include "ina219.h"
#include "input.h"
#include "motor.h"
#include "runtime_status.h"
#include "rv3029.h"
#include "sigfox.h"
#include "solar.h"
#include "web_routes_ui.h"

#define UI_TAG "webui"

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");
extern const uint8_t app_css_start[] asm("_binary_app_css_start");
extern const uint8_t app_css_end[] asm("_binary_app_css_end");
extern const uint8_t app_js_start[] asm("_binary_app_js_start");
extern const uint8_t app_js_end[] asm("_binary_app_js_end");

static esp_err_t send_json_response(httpd_req_t *req, const char *json)
{
	httpd_resp_set_type(req, "application/json");
	httpd_resp_set_hdr(req, "Cache-Control", "no-store");
	return httpd_resp_sendstr(req, json);
}

static esp_err_t serve_asset(httpd_req_t *req, const char *content_type, const uint8_t *start, const uint8_t *end)
{
	size_t content_len = end > start ? (size_t) (end - start - 1) : 0;

	httpd_resp_set_type(req, content_type);
	httpd_resp_set_hdr(req, "Cache-Control", "no-store");
	return httpd_resp_send(req, (const char *) start, content_len);
}

static bool parse_hhmm(const char *value, uint8_t *hours, uint8_t *minutes)
{
	int parsed_hours;
	int parsed_minutes;

	if (value == NULL || strlen(value) != 5 || value[2] != ':')
		return false;

	if (sscanf(value, "%2d:%2d", &parsed_hours, &parsed_minutes) != 2)
		return false;

	if (parsed_hours < 0 || parsed_hours > 23 || parsed_minutes < 0 || parsed_minutes > 59)
		return false;

	*hours = parsed_hours;
	*minutes = parsed_minutes;
	return true;
}

static bool parse_hour_value(const char *value, uint8_t *hours)
{
	char *endptr = NULL;
	long parsed = strtol(value, &endptr, 10);

	if (value == NULL || *value == '\0' || endptr == NULL || *endptr != '\0')
		return false;

	if (parsed < 0 || parsed > 25)
		return false;

	*hours = parsed;
	return true;
}

static void reboot_task(void *arg)
{
	vTaskDelay(pdMS_TO_TICKS(250));
	esp_restart();
}

static esp_err_t schedule_reboot(void)
{
	BaseType_t created = xTaskCreate(reboot_task, "web_reboot", 2048, NULL, 5, NULL);
	return created == pdPASS ? ESP_OK : ESP_FAIL;
}

static esp_err_t status_get_handler(httpd_req_t *req)
{
	uint8_t seconds = 0;
	uint8_t minutes = 0;
	uint8_t hours = 0;
	uint8_t alarm_minutes = 0;
	uint8_t alarm_hours = 0;
	uint8_t autoclose_hours = 0;
	button_state_t button_state = BUTTON_NONE;
	desired_door_state_t desired_state = DESIRED_DOOR_STATE_USER;
	bool desired_state_reached = false;
	bool door_down_reached = false;
	bool door_up_reached = false;
	double temperature = 0;
	double pressure = 0;
	double humidity = 0;
	float battery_voltage = 0;
	float current_amps = 0;
	float motor_power = 0;
	bool reed_closed;
	char json[1024];

	ESP_RETURN_ON_ERROR(rv3029_gettime(&seconds, &minutes, &hours), UI_TAG, "Failed to read RTC time");
	ESP_RETURN_ON_ERROR(alarmnvs_retrieve_time(&alarm_minutes, &alarm_hours), UI_TAG, "Failed to read alarm time");
	ESP_RETURN_ON_ERROR(alarmnvs_retrieve_autoclose_hours(&autoclose_hours), UI_TAG, "Failed to read autoclose hour");
	ESP_RETURN_ON_ERROR(desired_door_state_retrieve(&desired_state), UI_TAG, "Failed to read desired state");
	ESP_RETURN_ON_ERROR(desired_door_state_is_reached(desired_state, &desired_state_reached), UI_TAG, "Failed to read reached state");
	ESP_RETURN_ON_ERROR(desired_door_state_retrieve_down_reached(&door_down_reached), UI_TAG, "Failed to read down reached flag");
	ESP_RETURN_ON_ERROR(desired_door_state_is_reached(DESIRED_DOOR_STATE_UP, &door_up_reached), UI_TAG, "Failed to read up reached flag");

	(void) bme280adaptor_get_data(&temperature, &pressure, &humidity);
	(void) ina219_current(&current_amps);
	(void) ina219_power(&motor_power);
	battery_voltage = battery_get_voltage();
	reed_closed = input_get_reed_state();
	button_state = input_get_button_state();

	snprintf(json, sizeof(json),
			"{"
			"\"time\":\"%02u:%02u:%02u\","
			"\"timeHours\":%u,"
			"\"timeMinutes\":%u,"
			"\"alarm\":\"%02u:%02u\","
			"\"alarmHours\":%u,"
			"\"alarmMinutes\":%u,"
			"\"autocloseHours\":%u,"
			"\"desiredDoorState\":\"%s\","
			"\"desiredDoorStateReached\":%s,"
			"\"doorUpReached\":%s,"
			"\"doorDownReached\":%s,"
			"\"reedClosed\":%s,"
			"\"buttonState\":\"%s\","
			"\"wakeReason\":\"%s\","
			"\"motorState\":\"%s\","
			"\"batteryVoltage\":%.2f,"
			"\"motorCurrent\":%.3f,"
			"\"motorPower\":%.3f,"
			"\"temperature\":%.2f,"
			"\"pressure\":%.2f,"
			"\"humidity\":%.2f,"
			"\"solarCharging\":%s,"
			"\"solarFault\":%s,"
			"\"sigfoxAuthoritative\":true"
			"}",
			hours, minutes, seconds,
			hours,
			minutes,
			alarm_hours, alarm_minutes,
			alarm_hours,
			alarm_minutes,
			autoclose_hours,
			desired_door_state_to_string(desired_state),
			desired_state_reached ? "true" : "false",
			door_up_reached ? "true" : "false",
			door_down_reached ? "true" : "false",
			reed_closed ? "true" : "false",
			button_state == BUTTON_UP ? "UP" :
			button_state == BUTTON_DOWN ? "DOWN" :
			button_state == BUTTON_BOTH ? "BOTH" : "NONE",
			runtime_status_wakeup_reason_to_string(runtime_status_get_last_wakeup_reason()),
			motor_state_to_string(motor_get_state()),
			battery_voltage,
			current_amps,
			motor_power,
			temperature,
			pressure,
			humidity,
			solar_is_charging() ? "true" : "false",
			solar_is_fault() ? "true" : "false");

	return send_json_response(req, json);
}

static esp_err_t reboot_post_handler(httpd_req_t *req)
{
	char body[160];
	int received = 0;
	char mode[16];

	if (req->content_len <= 0 || req->content_len >= sizeof(body)) {
		httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Request body too large");
		return ESP_FAIL;
	}

	while (received < req->content_len) {
		int chunk_len = httpd_req_recv(req, body + received, req->content_len - received);
		if (chunk_len == HTTPD_SOCK_ERR_TIMEOUT)
			continue;
		if (chunk_len <= 0) {
			httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive request body");
			return ESP_FAIL;
		}
		received += chunk_len;
	}
	body[received] = '\0';

	if (httpd_query_key_value(body, "mode", mode, sizeof(mode)) != ESP_OK) {
		httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing mode");
		return ESP_FAIL;
	}

	if (strcmp(mode, "save") == 0) {
		uint8_t time_hours = 0;
		uint8_t time_minutes = 0;
		uint8_t alarm_hours = 0;
		uint8_t alarm_minutes = 0;
		uint8_t autoclose_hours = 0;
		char time_value[6];
		char alarm_value[6];
		char autoclose_value[4];

		if (httpd_query_key_value(body, "current_time", time_value, sizeof(time_value)) != ESP_OK ||
				!parse_hhmm(time_value, &time_hours, &time_minutes)) {
			httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid current_time");
			return ESP_FAIL;
		}

		if (httpd_query_key_value(body, "alarm_time", alarm_value, sizeof(alarm_value)) != ESP_OK ||
				!parse_hhmm(alarm_value, &alarm_hours, &alarm_minutes)) {
			httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid alarm_time");
			return ESP_FAIL;
		}

		if (httpd_query_key_value(body, "autoclose_hour", autoclose_value, sizeof(autoclose_value)) != ESP_OK ||
				!parse_hour_value(autoclose_value, &autoclose_hours)) {
			httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid autoclose_hour");
			return ESP_FAIL;
		}

		ESP_RETURN_ON_ERROR(rv3029_settime(0, time_minutes, time_hours), UI_TAG, "Failed to set RTC time");
		ESP_RETURN_ON_ERROR(alarmnvs_store_time(alarm_minutes, alarm_hours), UI_TAG, "Failed to store alarm time");
		ESP_RETURN_ON_ERROR(rv3029_set_alarm_minutes(alarm_minutes), UI_TAG, "Failed to update RTC alarm minute");
		ESP_RETURN_ON_ERROR(alarmnvs_store_autoclose_hours(autoclose_hours), UI_TAG, "Failed to store autoclose hour");

		ESP_LOGI(UI_TAG, "Saved web config before reboot: time=%02u:%02u alarm=%02u:%02u autoclose=%02u",
				time_hours, time_minutes, alarm_hours, alarm_minutes, autoclose_hours);

		ESP_RETURN_ON_ERROR(schedule_reboot(), UI_TAG, "Failed to schedule reboot");
		return send_json_response(req,
				"{\"ok\":true,\"message\":\"Settings saved locally. Rebooting now. Sigfox downlink will still override them later.\"}");
	}

	if (strcmp(mode, "discard") == 0) {
		ESP_RETURN_ON_ERROR(schedule_reboot(), UI_TAG, "Failed to schedule reboot");
		return send_json_response(req,
				"{\"ok\":true,\"message\":\"Discarded unsaved web changes. Rebooting now.\"}");
	}

	httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid mode");
	return ESP_FAIL;
}

static esp_err_t sigfox_post_handler(httpd_req_t *req)
{
	ESP_RETURN_ON_ERROR(sigfox_report(true, false), UI_TAG, "Sigfox report failed");
	return send_json_response(req,
			"{\"ok\":true,\"message\":\"Sigfox uplink + downlink sequence completed.\"}");
}

static esp_err_t index_get_handler(httpd_req_t *req)
{
	return serve_asset(req, "text/html; charset=utf-8", index_html_start, index_html_end);
}

static esp_err_t css_get_handler(httpd_req_t *req)
{
	return serve_asset(req, "text/css; charset=utf-8", app_css_start, app_css_end);
}

static esp_err_t js_get_handler(httpd_req_t *req)
{
	return serve_asset(req, "application/javascript; charset=utf-8", app_js_start, app_js_end);
}

esp_err_t web_routes_ui_register(httpd_handle_t server)
{
	static const httpd_uri_t index_get = {
		.uri = "/",
		.method = HTTP_GET,
		.handler = index_get_handler,
		.user_ctx = NULL
	};
	static const httpd_uri_t css_get = {
		.uri = "/app.css",
		.method = HTTP_GET,
		.handler = css_get_handler,
		.user_ctx = NULL
	};
	static const httpd_uri_t js_get = {
		.uri = "/app.js",
		.method = HTTP_GET,
		.handler = js_get_handler,
		.user_ctx = NULL
	};
	static const httpd_uri_t status_get = {
		.uri = "/api/status",
		.method = HTTP_GET,
		.handler = status_get_handler,
		.user_ctx = NULL
	};
	static const httpd_uri_t reboot_post = {
		.uri = "/api/reboot",
		.method = HTTP_POST,
		.handler = reboot_post_handler,
		.user_ctx = NULL
	};
	static const httpd_uri_t sigfox_post = {
		.uri = "/api/sigfox-sync",
		.method = HTTP_POST,
		.handler = sigfox_post_handler,
		.user_ctx = NULL
	};

	ESP_RETURN_ON_ERROR(httpd_register_uri_handler(server, &index_get), UI_TAG, "Failed to register index");
	ESP_RETURN_ON_ERROR(httpd_register_uri_handler(server, &css_get), UI_TAG, "Failed to register css");
	ESP_RETURN_ON_ERROR(httpd_register_uri_handler(server, &js_get), UI_TAG, "Failed to register js");
	ESP_RETURN_ON_ERROR(httpd_register_uri_handler(server, &status_get), UI_TAG, "Failed to register status");
	ESP_RETURN_ON_ERROR(httpd_register_uri_handler(server, &reboot_post), UI_TAG, "Failed to register reboot");
	ESP_RETURN_ON_ERROR(httpd_register_uri_handler(server, &sigfox_post), UI_TAG, "Failed to register sigfox");

	return ESP_OK;
}
