#ifndef WEB_ROUTES_UI_H
#define WEB_ROUTES_UI_H

#include <esp_err.h>
#include <esp_http_server.h>

esp_err_t web_routes_ui_register(httpd_handle_t server);

#endif
