#ifndef __USART_H
#define __USART_H

#include "main.h"

// 定义帧头和协议相关的固定内容
#define FRAME_HEADER           0xFF, 0xFF, 0x06, 0x09 // 帧头       0-3
#define FRAME_NUMBER           0x00, 0x00             // 帧序列号   4-5
#define FRAME_LENGTH           0x00, 0x00             // 帧长度     6-7
#define DEVICE_ID              0x33, 0x30, 0x06, 0x66 // 设备 ID    8-11
#define TARGET_ID              0x12, 0x34, 0x56, 0x78 // 目标 ID    12-15
#define RESPONSE_CODE          0x00, 0x00             // 回复       16-17
#define STATUS_CODE            0x00, 0x01             // 状态字     18-19  53 59 43 33 33 33 33 2d 36 36
#define SYC_STRING             0x53, 0x59, 0x43, 0x33, 0x33, 0x33, 0x33, 0x2D, 0x36, 0x36 // SYC3333-66     20-29
#define RESERVED_SPACE         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 保留空间      30-49

extern UART_HandleTypeDef        UartHandle; // 串口句柄
extern ADC_HandleTypeDef         AdcHandle;  // ADC句柄
extern TIM_HandleTypeDef         TimHandle;  // 定时器句柄
extern ADC_ChannelConfTypeDef    sConfig;    // ADC通道扫描句柄

// 引用数组必须写清楚数组大小
extern uint8_t Generic_Reply[50];
extern uint8_t UDP_11 [56];
extern uint8_t UDP_15 [20];
extern uint8_t UDP_71 [];
extern uint32_t G_USART1_RX_Count;         // 串口计数
extern uint8_t  G_USART1_RX_Buffer[64];    // 串口缓冲区
extern volatile uint16_t  G_setFlag;       // 串口标记位
extern volatile uint8_t   G_USART1_RX_Flag;// 空闲中断标记;
extern volatile float prev_value;          // 防止与上一次数据一样
extern volatile uint8_t UART_RX_CMD, UART_RX_CMD_1, UART_RX_CMD_3, UART_RX_CMD_5, UART_RX_CMD_11, UART_RX_CMD_13, UART_RX_CMD_15, UART_RX_CMD_71; // FF协议收发
extern float cxxx0[4];                     // RM0寄生电容值
extern float cxxx1[4];                     // RM1寄生电容值
extern float cxxx2[4];                     // RM2寄生电容值
extern float cxxx3[4];                     // RM3寄生电容值
extern char AT_Rename[50];                 // 蓝牙名称

// 函数定义
void USART1_GPIO_Init(void);
void USART1_Init(uint32_t baudrate);
void process_received_data(void);
void process_value(float value, uint8_t *udp_array, int start_index);
void read_and_store_values(uint8_t addr,int offset) ;
void USART1_Send_String(char *str);
void USART1_Print_Received_Data(void);
void AT_Program(void);
#endif
