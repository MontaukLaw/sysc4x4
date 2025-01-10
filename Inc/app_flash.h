#ifndef __APP_FLASH_H_
#define __APP_FLASH_H_

#include "py32f0xx_hal.h"
#include "tim.h"

// #include "py32f002bxx_ll_Start_Kit.h"

// #include "py32f002b_ll_rcc.h"
// #include "py32f002b_ll_bus.h"
// #include "py32f002b_ll_system.h"
// #include "py32f002b_ll_cortex.h"
// #include "py32f002b_ll_utils.h"
// #include "py32f002b_ll_pwr.h"
// #include "py32f002b_ll_gpio.h"
// #include "py32f002b_ll_rcc.h"

#define FLASH_USER_START_ADDR 0x08005F80 // 0x08005F00
#define FF_FLASH_BUF_LEN 16

void test_flash(void);
void app_read_data(void);

extern float f_flash_buffer[];

#endif // __APP_FLASH_H_