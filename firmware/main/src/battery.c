#include <esp_adc_cal.h>
#include <driver/adc.h>

#define BATT_ADC_CHANNEL ADC1_CHANNEL_0

#define BATT_RES_H 820
#define BATT_RES_L 220

static esp_adc_cal_characteristics_t *adc_chars;

esp_err_t battery_init(void)
{
	esp_err_t res = ESP_OK;

	res |= adc1_config_width(ADC_WIDTH_BIT_12);
	res |= adc1_config_channel_atten(BATT_ADC_CHANNEL, ADC_ATTEN_DB_11);
	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
	esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);

	return res;
}

float battery_get_voltage(void)
{
	int batt_adc_mv = esp_adc_cal_raw_to_voltage(adc1_get_raw(BATT_ADC_CHANNEL), adc_chars);

	return 0.001 * batt_adc_mv * (BATT_RES_H + BATT_RES_L) / BATT_RES_L;
}
