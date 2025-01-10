#include "usart.h"
#include "py32f0xx_hal.h"
#define CALI_PACK_LEN 30
#define NORMAL_CMD_LEN 20

UART_HandleTypeDef UartHandle;  // 串口句柄
ADC_HandleTypeDef AdcHandle;    // ADC句柄
ADC_ChannelConfTypeDef sConfig; // ADC通道扫描句柄

volatile uint16_t G_setFlag = 0x00;                                                                                                                                        // 状态标识
volatile uint8_t G_USART1_RX_Flag = 0;                                                                                                                                     // 空闲中断标记
volatile float prev_value = 0.0;                                                                                                                                           // 防止与上一次数据一样
uint8_t G_USART1_RX_Buffer[64] = {0};                                                                                                                                      // 串口1接收缓冲区
uint32_t G_USART1_RX_Count = 0;                                                                                                                                            // 串口1接收计数
volatile uint8_t UART_RX_CMD = 0, UART_RX_CMD_1 = 0, UART_RX_CMD_3 = 0, UART_RX_CMD_5 = 0, UART_RX_CMD_11 = 0, UART_RX_CMD_13 = 0, UART_RX_CMD_15 = 0, UART_RX_CMD_71 = 0; // FF协议收发
char AT_Rename[50];

/* 协议发送-------------------------------------------------------------------*/
uint8_t Generic_Reply[] = {
    FRAME_HEADER // 帧头
    ,
    FRAME_NUMBER // 帧序号
    ,
    FRAME_LENGTH // 帧长
    ,
    DEVICE_ID // 设备ID
    ,
    TARGET_ID // 目标ID
    ,
    RESPONSE_CODE // 命令字
    ,
    STATUS_CODE // 数据
    ,
    SYC_STRING // 设备类型
    ,
    RESERVED_SPACE // 保留
};

uint8_t UDP_11[] = {
    FRAME_HEADER // 0,1,2,3         固定：帧头
    ,
    0x00, 0x00 // 4,5             变量：帧序列号
    ,
    0x00, 0x30 // 6,7             固定：帧长
    ,
    DEVICE_ID // 8,9,10,11       固定：设备ID
    ,
    TARGET_ID // 12,13,14,15     固定：目标ID
    ,
    0x00, 0x12 // 16,17           固定：命令字
    ,
    0x00, 0x06 // 18,19           固定：状态字---传感器个数
    /* RM0 */
    ,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 20,21,22,23,24,25
    ,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 26,27,28,29,30,31
    ,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 32,33,34,35,36,37
    ,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 38,39,40,41,42,43
    /* RM1 */
    ,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 44,45,46,47,48,49
    ,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 50,51,52,53,54,55
};

uint8_t UDP_15[] = {
    FRAME_HEADER, 0x00, 0x01, 0x00, 0x0c, DEVICE_ID, TARGET_ID, 0x00, 0x16, 0x00, 0x03};

uint8_t UDP_71[CAILI_FB_LEN_BYTES] = {
    FRAME_HEADER // 0-3
    ,
    0x00, 0x00 // 4-5
    ,
    0x00, 0x16 // 6-7
    ,
    DEVICE_ID // 8-11
    ,
    TARGET_ID // 12-15
    ,
    0x00, 0x72 // 16-17
    ,
    0x00, 0x00 // 通道号 18-19
    ,
    0x00, 0x01 // 状态字 20-21
    ,
    0x00, 0x00 // type 22-23
    ,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 归一化系数 24-35
};

// USART1 GPIO初始化
void USART1_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE(); // 开启GPIOA模块时钟

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;     // PA3|PA4
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;            // 推免复用
    GPIO_InitStruct.Pull = GPIO_PULLUP;                // 上拉模式
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 超高速模式
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;       // 连接到USART1

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); // 根据GPIO结构体初始化引脚
}

// USART1初始化
void USART1_Init(uint32_t baudrate)
{
    /*使能时钟*/
    __HAL_RCC_USART1_CLK_ENABLE(); // 开启USART1模块时钟

    /*配置USART1参数*/
    UartHandle.Instance = USART1;                    // 串口基地址为USART1
    UartHandle.Init.BaudRate = baudrate;             // 波特率设置为115200
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B; // 数据位
    UartHandle.Init.StopBits = UART_STOPBITS_1;      // 停止位
    UartHandle.Init.Parity = UART_PARITY_NONE;       // 无校验
    UartHandle.Init.Mode = UART_MODE_TX_RX;          // 工作模式选择发送与接收
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件控制流

    HAL_UART_Init(&UartHandle);                      // 根据结构体配置初始化USART1
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_IDLE); // 空闲中断使能

    /********************************************************************/
    // 由于HAL的机制 比较难用也难理解 所以这里采用操作寄存器的方式
    USART1->CR1 |= (1 << 2); // 使能 接收
    USART1->CR1 |= (1 << 3); // 使能 发送
    USART1->CR1 |= (1 << 4); // 使能 空闲中断
    USART1->CR1 |= (1 << 5); // 使能 接收中断

    USART1->DR &= 0x00; // 清空数据寄存器

    USART1_GPIO_Init(); // USART1 GPIO初始化

    /* 使能NVIC */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

// 串口中断函数
void USART1_IRQHandler(void)
{
    if ((USART1->SR & (1 << 5)) != 0) // 接收存入数组中
    {
        if (G_USART1_RX_Count < 256) // 防止溢出
        {
            G_USART1_RX_Buffer[G_USART1_RX_Count] = (uint8_t)USART1->DR; // 读取数据
            G_USART1_RX_Count++;
            /* 清除标记位 */
            USART1->SR &= ~(1 << 5);
        }
    }
    if ((USART1->SR & (1 << 4)) != 0) // 产生空闲中断
    {
        G_USART1_RX_Flag = 1; // 接收完成标志置1
        /*清除中断标志位*/
        USART1->SR;
        USART1->DR;
    }
}

// 配置蓝牙名字的AT指令
void AT_Program(void)
{
    char AT_check[] = "AT+UART_DEF?\r\n";
    char AT_Baud[] = "AT+UART_DEF=230400,8,1,0,0\r\n";
    char AT_Name[] = "AT+BLENAME?\r\n";
    char AT_BLEADDR[] = "AT+BLEADDR?\r\n";
    USART1_Send_String(AT_Baud); // 设置波特率
    HAL_Delay(100);
    USART1_Send_String(AT_BLEADDR); // 查询蓝牙地址
    HAL_Delay(100);
    USART1_Send_String(AT_Rename); // 修改蓝牙名字
    HAL_Delay(100);
    USART1_Send_String(AT_Baud); // 设置波特率
    HAL_Delay(100);
    USART1_Print_Received_Data();

    USART1_Send_String(AT_check); // 查询波特率
    HAL_Delay(100);
    USART1_Print_Received_Data();
    HAL_Delay(100);
    USART1_Send_String(AT_Name); // 查询蓝牙名字
    HAL_Delay(100);
    USART1_Print_Received_Data();
    HAL_Delay(100);
    USART1_Send_String(AT_Rename); // 修改蓝牙名字
    HAL_Delay(100);
    USART1_Print_Received_Data();
    HAL_Delay(100);
    USART1_Send_String(AT_Name); // 查询蓝牙名字
    HAL_Delay(100);
    USART1_Print_Received_Data();
}

// 串口接收并打印数据的函数
void USART1_Print_Received_Data(void)
{
    // 检查是否接收完成标志为1
    if (G_USART1_RX_Flag == 1)
    {
        // 打印接收到的完整数据
        printf("\r\n%s\r\n", G_USART1_RX_Buffer);
        // 检查是否包含 "+BLEADDR"
        if (strstr((char *)G_USART1_RX_Buffer, "+BLEADDR") != NULL)
        {
            // printf("%c%c%c%c\r\n", G_USART1_RX_Buffer[28], G_USART1_RX_Buffer[29],G_USART1_RX_Buffer[31], G_USART1_RX_Buffer[32]);
            // 组合蓝牙名称：前缀 + 后4位蓝牙地址
            snprintf(AT_Rename, sizeof(AT_Rename), "AT+BLENAME=\"SCM 4*4_%c%c%c%c\"\r\n",
                     G_USART1_RX_Buffer[28], G_USART1_RX_Buffer[29],
                     G_USART1_RX_Buffer[31], G_USART1_RX_Buffer[32]);
        }
        // 清除标志位和缓冲区
        G_USART1_RX_Flag = 0;                                      // 清除接收完成标志
        G_USART1_RX_Count = 0;                                     // 重置接收计数
        memset(G_USART1_RX_Buffer, 0, sizeof(G_USART1_RX_Buffer)); // 清空接收缓冲区
    }
}

void cali_cmd_parser(void)
{
    uint8_t cmd_id = G_USART1_RX_Buffer[17];
    switch (cmd_id)
    {
    case 0x01:
        UART_RX_CMD_1 = 1;
        break;
    case 0x03:
        UART_RX_CMD_3 = 1;
        break;
    case 0x05:
        UART_RX_CMD_5 = 1;
        break;
    case 0x11:
        UART_RX_CMD_11 = 1;
        break;
    case 0x13:
        UART_RX_CMD_13 = 1;
        break;
    case 0x15:
        UART_RX_CMD_15 = 1;
        break;
    }
}

void normal_cmd_parser(void)
{
    uint8_t cmd_id = G_USART1_RX_Buffer[17];
}

// 解包函数
void process_received_data(void)
{
    uint8_t cmd_id = 0;
    // printf("G_USART1_RX_Count:%d\r\n", G_USART1_RX_Count);
    if (G_USART1_RX_Buffer[0] != 0xFF || G_USART1_RX_Buffer[1] != 0xFF || G_USART1_RX_Buffer[2] != 0x06 || G_USART1_RX_Buffer[3] != 0x09)
    {
        return;
    }

    if (G_USART1_RX_Count == NORMAL_CMD_LEN)
    {
        cmd_id = G_USART1_RX_Buffer[17];

        switch (cmd_id)
        {
        case 0x01:
            UART_RX_CMD_1 = 1;
            break;
        case 0x03:
            UART_RX_CMD_3 = 1;
            break;
        case 0x05:
            UART_RX_CMD_5 = 1;
            break;
        case 0x11:
            UART_RX_CMD_11 = 1;
            break;
        case 0x13:
            UART_RX_CMD_13 = 1;
            break;
        case 0x15:
            UART_RX_CMD_15 = 1;
            break;
        }

        UART_RX_CMD = cmd_id;

        G_USART1_RX_Count = 0;
    }
    else if (G_USART1_RX_Count == CALI_PACK_LEN)
    {
        cmd_id = G_USART1_RX_Buffer[17];

        if (cmd_id == 0x71)
        {
            UART_RX_CMD_71 = 1;
        }

        UART_RX_CMD = cmd_id;

        G_USART1_RX_Count = 0;
    }

    // 安全措施
    if (G_USART1_RX_Count > CALI_PACK_LEN)
    {
        G_USART1_RX_Count = 0;
    }

#if 0
    // uint16_t USART1_RX_Len = sizeof((char *)G_USART1_RX_Buffer); // 计算接收到的数据长度
    if (G_USART1_RX_Count > 19)
    {
        if (G_USART1_RX_Buffer[0] == 0x23 && G_USART1_RX_Buffer[1] == 0x24)
        {
            // 根据命令字设置标志位
            switch (G_USART1_RX_Buffer[2])
            {
            case 0x01:
                G_setFlag = 0x01; // printf打印
                break;
            case 0x02:
                G_setFlag = 0x02; // 串口输出
                break;
            case 0xF0:
                G_setFlag = 0xF0; // 通道0校准
                prev_value = 0.0; // 防止与上一次数据一样
                break;
            case 0xF1:
                G_setFlag = 0xF1;
                prev_value = 0.0;
                break;
            case 0xF2:
                G_setFlag = 0xF2;
                prev_value = 0.0;
                break;
            case 0xF3:
                G_setFlag = 0xF3;
                prev_value = 0.0;
                break;
            case 0xFF:
                G_setFlag = 0xFF; // 全通道校准
                break;
            }
        }
        else if (G_USART1_RX_Buffer[0] == 0xFF && G_USART1_RX_Buffer[1] == 0xFF && G_USART1_RX_Buffer[2] == 0x06 && G_USART1_RX_Buffer[3] == 0x09)
        {
            uint8_t cmd_id = G_USART1_RX_Buffer[17];

            // 处理第二种数据包
            switch (cmd_id)
            {
            case 0x01:
                UART_RX_CMD_1 = 1;
                break;
            case 0x03:
                UART_RX_CMD_3 = 1;
                break;
            case 0x05:
                UART_RX_CMD_5 = 1;
                break;
            case 0x11:
                UART_RX_CMD_11 = 1;
                break;
            case 0x13:
                UART_RX_CMD_13 = 1;
                break;
            case 0x15:
                UART_RX_CMD_15 = 1;
                break;
            case 0x71: // 新增的协议处理
                UART_RX_CMD_71 = 1;
                break;
            }

            UART_RX_CMD = cmd_id; // G_USART1_RX_Buffer[17];
        }

        // memset(G_USART1_RX_Buffer, 0, sizeof(G_USART1_RX_Buffer)); // 清空接收缓冲区
        G_USART1_RX_Count = 0;
        // G_USART1_RX_Flag  = 0;
    }
#endif
}

// 浮点数转换成十六进制
void process_value(float value, uint8_t *udp_array, int start_index)
{
    int int_part = (int)value;                                              // 整数部分
    float flo_part = value - int_part;                                      // 小数部分
    unsigned long int flo_to_int = (unsigned long int)(flo_part * 1000000); // 放大成整数处理

    udp_array[start_index] = (int_part & 0xFF0000) >> 16; // 整数部分
    udp_array[start_index + 1] = (int_part & 0x00FF00) >> 8;
    udp_array[start_index + 2] = (int_part & 0x0000FF);
    udp_array[start_index + 3] = (flo_to_int & 0xFF0000) >> 16; // 小数部分
    udp_array[start_index + 4] = (flo_to_int & 0x00FF00) >> 8;
    udp_array[start_index + 5] = (flo_to_int & 0x0000FF);
}

// 读取电容值
void read_and_store_values(uint8_t addr, int offset)
{
    c_read_value = rm1002_debug_func(addr, cxxx0, cxxx1, cxxx2, cxxx3);
    for (int i = 0; i < 4; i++)
    {
        c_value[offset + i] = c_read_value[i];
    }
}

// 串口发送数据函数（1字节）
void USART1_Send_Byte(uint8_t dat)
{
    while ((USART1->SR & (1 << 6)) == 0)
        ;             // 等待上一个数据发送完成
    USART1->DR = dat; // 将数据写入DR寄存器
}

// 串口发送字符串
void USART1_Send_String(char *str)
{
    while (*str) // 当字符串不为空
    {
        USART1_Send_Byte(*str); // 发送当前字符
        str++;                  // 移动到下一个字符
    }
}

// 重定向printf
int fputc(int ch, FILE *f)
{
    USART1_Send_Byte(ch);
    return ch;
}

// 串口错误处理函数
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    printf("Uart Error, ErrorCode = %d\r\n", huart->ErrorCode);
}
