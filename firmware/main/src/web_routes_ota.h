#ifndef WEB_ROUTES_OTA_H
#define WEB_ROUTES_OTA_H

#include <esp_err.h>
#include <esp_http_server.h>

esp_err_t web_routes_ota_register(httpd_handle_t server);

#endif
