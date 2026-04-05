#ifndef RUNTIME_STATUS_H
#define RUNTIME_STATUS_H

#include "input.h"

void runtime_status_set_last_wakeup_reason(wakeup_reason_t wakeup_reason);
wakeup_reason_t runtime_status_get_last_wakeup_reason(void);
const char *runtime_status_wakeup_reason_to_string(wakeup_reason_t wakeup_reason);

#endif
