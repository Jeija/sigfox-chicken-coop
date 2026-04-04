#include <stdlib.h>

#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>
#include <esp_err.h>

#define BATT_ADC_CHANNEL ADC_CHANNEL_0

#define BATT_RES_H 820
#define BATT_RES_L 220
#define TAG "battery"

static adc_oneshot_unit_handle_t adc_handle;
static adc_cali_handle_t adc_cali_handle;

esp_err_t battery_init(void)
{
	esp_err_t ret;

	adc_oneshot_unit_init_cfg_t unit_config = {
		.unit_id = ADC_UNIT_1,
	};
	ret = adc_oneshot_new_unit(&unit_config, &adc_handle);
	if (ret != ESP_OK) {
		return ret;
	}

	adc_oneshot_chan_cfg_t channel_config = {
		.bitwidth = ADC_BITWIDTH_12,
		.atten = ADC_ATTEN_DB_12,
	};
	ret = adc_oneshot_config_channel(adc_handle, BATT_ADC_CHANNEL, &channel_config);
	if (ret != ESP_OK) {
		return ret;
	}

	adc_cali_line_fitting_config_t cali_config = {
		.unit_id = ADC_UNIT_1,
		.atten = ADC_ATTEN_DB_12,
		.bitwidth = ADC_BITWIDTH_12,
		.default_vref = 1100,
	};

	return adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle);
}

float battery_get_voltage(void)
{
	int raw = 0;
	int batt_adc_mv = 0;

	if (adc_oneshot_read(adc_handle, BATT_ADC_CHANNEL, &raw) != ESP_OK) {
		return 0.0f;
	}
	if (adc_cali_raw_to_voltage(adc_cali_handle, raw, &batt_adc_mv) != ESP_OK) {
		return 0.0f;
	}

	return 0.001 * batt_adc_mv * (BATT_RES_H + BATT_RES_L) / BATT_RES_L;
}
