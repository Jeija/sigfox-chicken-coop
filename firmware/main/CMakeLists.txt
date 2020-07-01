FILE(GLOB main_sources src/*.c)

idf_component_register(SRCS ${main_sources}
	INCLUDE_DIRS "."
	REQUIRES soc driver spi_flash nvs_flash renard-phy-s2lp-container renard-phy-s2lp-hal-esp32 esp_adc_cal esp_http_server app_update
	EMBED_TXTFILES web/index.html
)
