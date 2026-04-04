FILE(GLOB main_sources src/*.c)
list(APPEND main_sources src/desired_door_state.c)

idf_component_register(SRCS ${main_sources}
	INCLUDE_DIRS "."
	REQUIRES soc driver spi_flash nvs_flash renard-phy-s2lp-container renard-phy-s2lp-hal-esp32 esp_adc esp_http_server app_update esp_wifi esp_event esp_netif esp_driver_ledc esp_driver_i2c esp_timer
	EMBED_TXTFILES web/index.html
)
