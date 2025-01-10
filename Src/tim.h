#ifndef __TIM_H
#define __TIM_H

#include "main.h"

extern TIM_HandleTypeDef TimHandle;  // 定时器句柄

extern volatile uint8_t   t_10ms_Flag;    // 10ms flag
extern volatile uint16_t  t_10ms_count;   // 10ms count
extern volatile uint32_t  hour_count;     // 一小时计数器
extern volatile uint8_t   hour_timeout_flag; // 超时标志

void APP_TimConfig(void);
void APP_ErrorHandler(void);
void Reset_Hour_Timer(void); // 添加重置计时器函数声明

#endif





