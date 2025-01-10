#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "py32f0xx_hal.h"
#include "py32f002bxx_Start_Kit.h"
#include "rm1002.h"
#include "usart.h"
#include "adjust.h"
#include "led.h"
#include "tim.h"
#include "iwdg.h"
#include "flash.h"
#include "arithmetic.h"
#include <string.h>

#define Vcc_Power           3.30                                            /* VCC power supply voltage, modify according to actual situation  */
#define TScal1              (float)((HAL_ADC_TSCAL1) * 3.3 / Vcc_Power)     /* Voltage corresponding to calibration value at 30 ℃ */
#define TScal2              (float)((HAL_ADC_TSCAL2) * 3.3 / Vcc_Power)     /* Voltage corresponding to calibration value at 85 ℃ */
#define TStem1              30                                              /* 30 ℃ */
#define TStem2              85                                              /* 85 ℃ */
#define Temp_k              ((float)(TStem2-TStem1)/(float)(TScal2-TScal1)) /* Temperature calculation */
#define DEFAULE_I2C_ADDR_0  (0x2B)                                          /* 锐盟地址 */
#define DEFAULE_I2C_ADDR_1  (0x2A) 
#define DEFAULE_I2C_ADDR_2  (0x29) 
#define DEFAULE_I2C_ADDR_3  (0x28) 
#define FLEX_FLAG 0

#define CAILI_FB_LEN_BYTES 30

extern uint8_t Generic_Reply[50];
extern uint8_t UDP_11 [56];
extern uint8_t UDP_15 [20];
extern uint8_t UDP_71 [];
extern TIM_HandleTypeDef  TimHandle;       // 定时器句柄
extern IWDG_HandleTypeDef IwdgHandle;      // 看门狗句柄
extern uint32_t G_USART1_RX_Count;         // 串口计数
extern uint8_t  G_USART1_RX_Buffer[64];    // 串口缓冲区
extern volatile uint16_t  G_setFlag;       // 串口标记位
extern volatile uint8_t   G_USART1_RX_Flag;// 空闲中断标记;
extern volatile float prev_value;          // 防止与上一次数据一样
extern volatile uint8_t UART_RX_CMD, UART_RX_CMD_1, UART_RX_CMD_3, UART_RX_CMD_5, UART_RX_CMD_11, UART_RX_CMD_13, UART_RX_CMD_15, UART_RX_CMD_71; // FF协议收发
extern volatile uint8_t   t_10ms_Flag;     // 10ms flag
extern volatile uint16_t  t_10ms_count;    // 10ms count
extern volatile uint32_t hour_count;       // 一小时计数器
extern volatile uint8_t hour_timeout_flag; // 超时标志
extern volatile uint8_t   RM1002B_X;       // RM地址
extern volatile float c_value[16];         // 传感器值
extern volatile uint8_t G_Printf_Flag;     // 20ms打印一次
extern float *c_read_value;                // 电容值
extern float cxxx0[4];                     // RM0寄生电容值
extern float cxxx1[4];                     // RM1寄生电容值
extern float cxxx2[4];                     // RM2寄生电容值
extern float cxxx3[4];                     // RM3寄生电容值

#endif /* __MAIN_H */
