#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/ledc.h>

#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_SPEED_MODE LEDC_HIGH_SPEED_MODE

esp_err_t buzzer_init(void)
{
	esp_err_t res = ESP_OK;

	ledc_timer_config_t timer_config = {
		.speed_mode = LEDC_SPEED_MODE,
		.duty_resolution = LEDC_TIMER_2_BIT,
		.timer_num = LEDC_TIMER_0,
		.freq_hz = 400
	};
	res |= ledc_timer_config(&timer_config);

	ledc_channel_config_t ledc_channel = {
		.channel = LEDC_CHANNEL,
		.duty = 0,
		.gpio_num = 17,
		.speed_mode = LEDC_SPEED_MODE,
		.timer_sel = LEDC_TIMER_0
	};
	res |= ledc_channel_config(&ledc_channel);

	return res;
}

esp_err_t buzzer_buzz(int freq, int duration)
{
	esp_err_t res = ESP_OK;

	res |= ledc_set_freq(LEDC_SPEED_MODE, LEDC_CHANNEL_0, freq);
	res |= ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0, 1);
	res |= ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0);

	vTaskDelay(duration / portTICK_PERIOD_MS);

	res |= ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0, 0);
	res |= ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0);

	return res;
}
