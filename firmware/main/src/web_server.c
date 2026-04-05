#include "web_server.h"

esp_err_t web_server_start(httpd_handle_t *server)
{
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();

	if (httpd_start(server, &config) != ESP_OK)
		return ESP_FAIL;

	return ESP_OK;
}

esp_err_t web_server_stop(httpd_handle_t server)
{
	return server != NULL ? httpd_stop(server) : ESP_OK;
}
