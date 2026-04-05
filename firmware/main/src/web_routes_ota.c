#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_http_server.h>
#include <esp_ota_ops.h>
#include <esp_system.h>
#include <sys/param.h>

#include "web_routes_ota.h"

static esp_err_t update_post_handler(httpd_req_t *req)
{
	char buf[1000];
	esp_ota_handle_t ota_handle;
	int remaining = req->content_len;

	const esp_partition_t *ota_partition = esp_ota_get_next_update_partition(NULL);
	ESP_ERROR_CHECK(esp_ota_begin(ota_partition, OTA_SIZE_UNKNOWN, &ota_handle));

	while (remaining > 0) {
		int recv_len = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)));

		if (recv_len == HTTPD_SOCK_ERR_TIMEOUT) {
			continue;
		} else if (recv_len <= 0) {
			httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Protocol Error");
			return ESP_FAIL;
		}

		if (esp_ota_write(ota_handle, (const void *) buf, recv_len) != ESP_OK) {
			httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Flash Error");
			return ESP_FAIL;
		}

		remaining -= recv_len;
	}

	if (esp_ota_end(ota_handle) != ESP_OK || esp_ota_set_boot_partition(ota_partition) != ESP_OK) {
		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Validation / Activation Error");
		return ESP_FAIL;
	}

	httpd_resp_sendstr(req, "Firmware update complete, rebooting now!");

	vTaskDelay(pdMS_TO_TICKS(500));
	esp_restart();
	return ESP_OK;
}

esp_err_t web_routes_ota_register(httpd_handle_t server)
{
	static const httpd_uri_t update_post = {
		.uri = "/update",
		.method = HTTP_POST,
		.handler = update_post_handler,
		.user_ctx = NULL
	};

	return httpd_register_uri_handler(server, &update_post);
}
