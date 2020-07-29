#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>
#include <string.h>

#include "renard_phy_s2lp_hal_platform.h"
#include "renard_phy_s2lp_protocol.h"
#include "renard_phy_s2lp.h"

#include "bme280adaptor.h"
#include "alarmnvs.h"
#include "battery.h"
#include "sigfox.h"
#include "rv3029.h"
#include "input.h"
#include "solar.h"

typedef struct {
	uint8_t magicbyte;
	uint8_t time_hours;
	uint8_t time_minutes;
	uint8_t alarm_hours;
	uint8_t alarm_minutes;
	uint8_t temperature;
	uint8_t pressure;
	uint8_t humidity;
	uint8_t battery;

	/*
	 * flag bits:
	 * (1 << 7): door closed (1) or open(0)
	 * (1 << 6): solar charging (1) or not charging (0)
	 * (1 << 5): solar fault (1) or no fault (0)
	 */
	uint8_t flags;
	uint8_t autoclose_hours;
} __attribute__((packed)) report_ul_t;

typedef struct {
	uint8_t time_hours;
	uint8_t time_minutes;
	uint8_t alarm_hours;
	uint8_t alarm_minutes;
	uint8_t autoclose_hours;
} __attribute__((packed)) config_dl_t;

uint8_t sigfox_key[] = {0x47, 0x9e, 0x44, 0x80, 0xfd, 0x75, 0x96, 0xd4, 0x5b, 0x01, 0x22, 0xfd, 0x28, 0x2d, 0xb3, 0xcf};
uint32_t sigfox_devid = 0x004d33db;

static uint16_t next_seqnum(void)
{
	/* Increment sequence number: Load current value from NVS, increment and store updated value */
	nvs_handle_t nvshandle;
	ESP_ERROR_CHECK(nvs_open("sigfox", NVS_READWRITE, &nvshandle));
	uint16_t seqnum = 0;
	nvs_get_u16(nvshandle, "seqnum", &seqnum);
	seqnum = (seqnum + 1) % 0xfff;
	nvs_set_u16(nvshandle, "seqnum", seqnum);

	return seqnum;
}

static uint8_t float_to_byte(float val, float min, float max)
{
	if (val >= max)
		return 255;
	else if (val <= min)
		return 0;

	return (uint8_t)(0xff * (val - min) / (max - min));
}

esp_err_t sigfox_init(void)
{
	esp_err_t res = ESP_OK;

	res |= renard_phy_s2lp_init() == true ? ESP_OK : ESP_FAIL;
	renard_phy_s2lp_protocol_init(esp_random(), UL_DATARATE_100BPS);

	// Add buttons and RTC as additional light sleep timeout sources
	renard_phy_s2lp_hal_add_timeout_source(CONFIG_GPIO_BUTTON_UP, GPIO_INTR_HIGH_LEVEL);
	renard_phy_s2lp_hal_add_timeout_source(CONFIG_GPIO_BUTTON_DOWN, GPIO_INTR_HIGH_LEVEL);
	renard_phy_s2lp_hal_add_timeout_source(CONFIG_GPIO_RTC_INT, GPIO_INTR_HIGH_LEVEL);

	return res;
}

esp_err_t sigfox_report(bool request_downlink, bool is_ack)
{
	/* Gather data to report */
	report_ul_t report;
	report.magicbyte = is_ack ? 0x69 : 0x68;

	// time
	ESP_ERROR_CHECK(rv3029_gettime(NULL, &report.time_minutes, &report.time_hours));

	// open time
	ESP_ERROR_CHECK(alarmnvs_retrieve_time(&report.alarm_minutes, &report.alarm_hours));
	ESP_ERROR_CHECK(alarmnvs_retrieve_autoclose_hours(&report.autoclose_hours));

	// weather
	double temperature, pressure, humidity;
	bme280adaptor_get_data(&temperature, &pressure, &humidity);
	printf("BME280 temp: %.2lf, press: %.2lf, humid: %.2lf\r\n", temperature, pressure, humidity);
	report.temperature = float_to_byte(temperature, -40, 60);
	report.pressure = float_to_byte(pressure, 98000, 100000);
	report.humidity = float_to_byte(humidity, 0, 100);

	// battery level
	report.battery = float_to_byte(battery_get_voltage(), 8, 14);

	// door + solar state flags
	report.flags = 0x00;
	if (input_get_reed_state())
		report.flags |= (1 << 7);
	if (solar_is_charging())
		report.flags |= (1 << 6);
	if (solar_is_fault())
		report.flags |= (1 << 5);

	/* Sigfox transmission */
	sfx_commoninfo common;
	common.seqnum = next_seqnum();
	common.devid = sigfox_devid;
	memcpy(common.key, sigfox_key, 16);

	sfx_ul_plain uplink;
	memcpy(uplink.payload, &report, sizeof(report));
	uplink.payloadlen = sizeof(report);
	uplink.request_downlink = request_downlink;
	uplink.singlebit = false;
	uplink.replicas = true;

	/* Actual protocol operation */
	sfx_dl_plain downlink;
	int16_t downlink_rssi;
	int err = renard_phy_s2lp_protocol_transfer(&common, &uplink, &downlink, &downlink_rssi);

	/* Log result */
	if (err == PROTOCOL_ERROR_NONE) {
		if (uplink.request_downlink) {
			printf("[renard-phy-s2lp-demo-esp32] Downlink received!\r\n");
			printf("[renard-phy-s2lp-demo-esp32] Downlink RSSI    : %d\r\n", downlink_rssi);
			printf("[renard-phy-s2lp-demo-esp32] Downlink payload : ");
			for (int i = 0; i < SFX_DL_PAYLOADLEN; i++)
				printf("%02x", downlink.payload[i]);
			printf("\r\n");
			printf("[renard-phy-s2lp-demo-esp32] Downlink CRC OK  : %d\r\n", downlink.crc_ok);
			printf("[renard-phy-s2lp-demo-esp32] Downlink MAC OK  : %d\r\n", downlink.mac_ok);
			printf("[renard-phy-s2lp-demo-esp32] Downlink FEC Use : %d\r\n", downlink.fec_corrected);

			// Apply downlink data
			config_dl_t config_dl;
			memcpy(&config_dl, &downlink.payload, sizeof(config_dl));
			ESP_ERROR_CHECK(rv3029_settime(0, config_dl.time_minutes, config_dl.time_hours));
			ESP_ERROR_CHECK(alarmnvs_store_time(config_dl.alarm_minutes, config_dl.alarm_hours));
			ESP_ERROR_CHECK(rv3029_set_alarm_minutes(config_dl.alarm_minutes));
			ESP_ERROR_CHECK(alarmnvs_store_autoclose_hours(config_dl.autoclose_hours));

			// Downlink confirmation message
			sigfox_report(false, true);
		} else {
			printf("[renard-phy-s2lp-demo-esp32] Uplink transmitted, no downlink requested.\r\n");
		}
	} else if (err == PROTOCOL_ERROR_TIMEOUT) {
		printf("[renard-phy-s2lp-demo-esp32] Timeout while waiting for downlink\r\n");
	} else {
		printf("[renard-phy-s2lp-demo-esp32] Unknown protocol error occurred\r\n");
		return ESP_FAIL;
	}

	return ESP_OK;
}
