#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <esp_err.h>
#include <esp_http_server.h>

esp_err_t web_server_start(httpd_handle_t *server);
esp_err_t web_server_stop(httpd_handle_t server);

#endif
