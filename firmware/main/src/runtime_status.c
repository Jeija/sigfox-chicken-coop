#include "runtime_status.h"

static wakeup_reason_t last_wakeup_reason = WAKEUP_REASON_UNKNOWN;

void runtime_status_set_last_wakeup_reason(wakeup_reason_t wakeup_reason)
{
	last_wakeup_reason = wakeup_reason;
}

wakeup_reason_t runtime_status_get_last_wakeup_reason(void)
{
	return last_wakeup_reason;
}

const char *runtime_status_wakeup_reason_to_string(wakeup_reason_t wakeup_reason)
{
	switch (wakeup_reason) {
		case WAKEUP_REASON_USER:
			return "USER";
		case WAKEUP_REASON_RTC:
			return "RTC";
		case WAKEUP_REASON_UNKNOWN:
		default:
			return "UNKNOWN";
	}
}
