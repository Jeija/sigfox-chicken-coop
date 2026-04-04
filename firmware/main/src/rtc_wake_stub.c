#include <stdbool.h>

#include <esp_attr.h>
#include <esp_rom_gpio.h>
#include <esp_sleep.h>
#include <hal/gpio_ll.h>
#include <soc/rtc_cntl_reg.h>
#include <soc/rtc_io_channel.h>
#include <soc/soc.h>
#include <sdkconfig.h>

#include "motor.h"

int rtc_wake_stub_force_link(void)
{
	return 1;
}

RTC_DATA_ATTR motor_state_t wake_stub_motor_request = MOTOR_OFF;

void RTC_IRAM_ATTR esp_wake_deep_sleep(void)
{
	esp_default_wake_deep_sleep();

	uint32_t ext1_status = REG_GET_FIELD(RTC_CNTL_EXT_WAKEUP1_STATUS_REG, RTC_CNTL_EXT_WAKEUP1_STATUS);
	bool button_up_pressed = (ext1_status & BIT(RTCIO_GPIO34_CHANNEL)) != 0;
	bool button_down_pressed = (ext1_status & BIT(RTCIO_GPIO35_CHANNEL)) != 0;

	if (button_up_pressed && !button_down_pressed) {
		wake_stub_motor_request = MOTOR_UP_UNTIL_LATCH;
	} else if (button_down_pressed && !button_up_pressed) {
		wake_stub_motor_request = MOTOR_DOWN_UNTIL_LATCH;
	} else {
		wake_stub_motor_request = MOTOR_OFF;
	}

	esp_rom_gpio_pad_select_gpio(CONFIG_GPIO_MOTOR_UP);
	esp_rom_gpio_pad_select_gpio(CONFIG_GPIO_MOTOR_DOWN);
	gpio_ll_output_enable(&GPIO, CONFIG_GPIO_MOTOR_UP);
	gpio_ll_output_enable(&GPIO, CONFIG_GPIO_MOTOR_DOWN);
	gpio_ll_set_level(&GPIO, CONFIG_GPIO_MOTOR_UP, button_up_pressed);
	gpio_ll_set_level(&GPIO, CONFIG_GPIO_MOTOR_DOWN, button_down_pressed);
}
