#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include <esp_event.h>
#include <esp_netif.h>
#include <esp_ota_ops.h>
#include <esp_system.h>
#include <esp_wifi.h>

#include "buzzer.h"
#include "ota.h"
#include "web_routes_ota.h"
#include "web_routes_ui.h"
#include "web_server.h"

#define WIFI_SSID "ESP32 OTA Update"

static esp_err_t softap_init(void)
{
	esp_err_t res = ESP_OK;

	res |= esp_netif_init();
	res |= esp_event_loop_create_default();
	esp_netif_create_default_wifi_ap();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	res |= esp_wifi_init(&cfg);

	wifi_config_t wifi_config = {
		.ap = {
			.ssid = WIFI_SSID,
			.ssid_len = strlen(WIFI_SSID),
			.channel = 6,
			.authmode = WIFI_AUTH_OPEN,
			.max_connection = 3
		},
	};

	res |= esp_wifi_set_mode(WIFI_MODE_AP);
	res |= esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
	res |= esp_wifi_start();

	return res;
}

static esp_err_t softap_deinit(void)
{
	esp_err_t res = ESP_OK;

	res |= esp_wifi_stop();
	res |= esp_wifi_deinit();

	return res;
}

esp_err_t ota_init(void)
{
	const esp_partition_t *partition = esp_ota_get_running_partition();
	printf("Currently running partition: %s\r\n", partition->label);

	esp_ota_img_states_t ota_state;
	if (esp_ota_get_state_partition(partition, &ota_state) == ESP_OK) {
		if (ota_state == ESP_OTA_IMG_PENDING_VERIFY)
			return esp_ota_mark_app_valid_cancel_rollback();
	}

	return ESP_OK;
}

void ota_mode(uint32_t timeout_ms)
{
	httpd_handle_t server = NULL;

	ESP_ERROR_CHECK(softap_init());
	ESP_ERROR_CHECK(web_server_start(&server));
	ESP_ERROR_CHECK(web_routes_ui_register(server));
	ESP_ERROR_CHECK(web_routes_ota_register(server));

	uint32_t total_ms = 0;
	while (total_ms < timeout_ms) {
		vTaskDelay(pdMS_TO_TICKS(100));
		buzzer_buzz(2000, 100);
		total_ms += 200;
	}

	ESP_ERROR_CHECK(web_server_stop(server));
	ESP_ERROR_CHECK(softap_deinit());
}
