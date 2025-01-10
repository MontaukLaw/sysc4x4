#ifndef __LED_H
#define __LED_H

#define RCC_SYSCLKSOURCE_HSI    0x00000000U                                     /*!< HSI selection as system clock */

#include "py32f0xx_hal.h"

void APP_LEDInit(void);
void System_Clock_Config_HSI_24Mhz(void);


#endif

