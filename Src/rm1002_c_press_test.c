#include "rm1002.h"
#include "main.h"

#if RM1002_C_PRESS_TEST

//#include "uartprintf.h"
#include "dev_config.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef I2cHandle;
#define I2C_ADDRESS      0xA0                /* Local address 0xA0 */
#define I2C_SPEEDCLOCK   4000000             /* Communication speed 400K */
#define I2C_DUTYCYCLE    I2C_DUTYCYCLE_16_9  /* Duty cycle */


// ch0
#define PROXTH_CH0_CLS_REAL_VAL    1000000
#define PROXTH_CH0_FAR_REAL_VAL    1000000
// ch1
#define PROXTH_CH1_CLS_REAL_VAL    1000000
#define PROXTH_CH1_FAR_REAL_VAL    1000000
// ch2
#define PROXTH_CH2_CLS_REAL_VAL    1000000
#define PROXTH_CH2_FAR_REAL_VAL    1000000
// ch3
#define PROXTH_CH3_CLS_REAL_VAL	   1000000
#define PROXTH_CH3_FAR_REAL_VAL	   1000000

#define AVG_TARGET_COMP_CH0_REAL_VAL    2000000
#define AVG_TARGET_COMP_CH1_REAL_VAL    2000000
#define AVG_TARGET_COMP_CH2_REAL_VAL    2000000
#define AVG_TARGET_COMP_CH3_REAL_VAL    2000000

// 通道0
#define PROXTH_CH0_CLS          (PROXTH_CH0_CLS_REAL_VAL >> 8)
#define PROXTH_CH0_FAR          (PROXTH_CH0_FAR_REAL_VAL >> 8)
// 通道1
#define PROXTH_CH1_CLS          (PROXTH_CH1_CLS_REAL_VAL >> 8)
#define PROXTH_CH1_FAR          (PROXTH_CH1_FAR_REAL_VAL >> 8)
// 通道2
#define PROXTH_CH2_CLS          (PROXTH_CH2_CLS_REAL_VAL >> 8)
#define PROXTH_CH2_FAR          (PROXTH_CH2_FAR_REAL_VAL >> 8)
// 通道3
#define PROXTH_CH3_CLS          (PROXTH_CH3_CLS_REAL_VAL >> 8)
#define PROXTH_CH3_FAR          (PROXTH_CH3_FAR_REAL_VAL >> 8)

#define AVG_TARGET_COMP_CH0     (AVG_TARGET_COMP_CH0_REAL_VAL >> 8)
#define AVG_TARGET_COMP_CH1     (AVG_TARGET_COMP_CH2_REAL_VAL >> 8)
#define AVG_TARGET_COMP_CH2     (AVG_TARGET_COMP_CH2_REAL_VAL >> 8)
#define AVG_TARGET_COMP_CH3     (AVG_TARGET_COMP_CH2_REAL_VAL >> 8)


#define RM1X01_PROXTH_CH0_CLS           0x49
#define RM1X01_PROXTH_CH0_FAR           0x51

#define RM1X01_RAW0_CH0                 0X75
#define RM1X01_USE0_CH0                 0X81
#define RM1X01_AVG0_CH0                 0X8D
#define RM1X01_DIF0_CH0                 0X99

typedef enum {
    RAW_DATA,
    USE_DATA,
    AVG_DATA,
    DIF_DATA
} RM1X01_DataType;

struct RM1X01_REG_CFG {
    uint8_t reg_address;
    uint8_t reg_value;
};

#if 1
struct RM1X01_REG_CFG rm1002_defaultcfg[] = {
    { 0x05, 0xf9 },
//  { 0x07, 0x0b },

    { 0x0c, 0x40 },
    { 0x0d, 0x40 },
    { 0x0e, 0x40 },
    { 0x0f, 0x40 },
    { 0x14, 0x83 },

    { 0x16, 0x0f },// 选择通道，CH0:0001  CH1:0010 CH2:0100 CH3:1000
    { 0x17, 0x00 },// Scan周期配置
    { 0x19, 0x06 },// ADC单次采样时间。值越小，变化越大
    { 0x1a, 0x06 },// ADC单次积分时间。速度

    { 0x1b, 0x00 },// CH0和CH1对应工作模式选择
    { 0x1c, 0x00 },// CH2和CH3对应工作模式选择
//  { 0x1c, 0x38 },

    { 0x1d, 0x02 },// CH0对应的物理通道配置
    { 0x1e, 0x08 },// CH1对应的物理通道配置
    { 0x1f, 0x20 },// CH2对应的物理通道配置
    { 0x20, 0x80 },// CH3对应的物理通道配置

    { 0x21, 0x03 },// 采样率，改小速度快，可能有噪声
    { 0x22, 0x03 },
    { 0x23, 0x03 },
    { 0x24, 0x03 },

    { 0x25, 0x00 },
    { 0x26, 0x00 },
    { 0x27, 0x00 },
    { 0x28, 0x00 },
    { 0x29, 0x00 },

    { 0x2a, 0x80 },
    { 0x2b, 0x81 },
    { 0x2c, 0x81 },
    { 0x2d, 0x81 },
    { 0x2e, 0x81 },

    { 0x2f, 0xff },// 0x00：使用人工设置的offset，0xFF：使能自动补偿
    { 0x30, 0x41 },
    { 0x31, 0xff & AVG_TARGET_COMP_CH0},
    { 0x32, 0xff & (AVG_TARGET_COMP_CH0 >> 8)},
    { 0x33, 0xff & AVG_TARGET_COMP_CH1},
    { 0x34, 0xff & (AVG_TARGET_COMP_CH1 >> 8)},
    { 0x35, 0xff & AVG_TARGET_COMP_CH2},
    { 0x36, 0xff & (AVG_TARGET_COMP_CH2 >> 8)},
    { 0x37, 0xff & AVG_TARGET_COMP_CH3},
    { 0x38, 0xff & (AVG_TARGET_COMP_CH3 >> 8)},

    { 0x39, 0x00 },
    { 0x3a, 0x00 },
    { 0x3b, 0x00 },
    { 0x3c, 0x00 },

    { 0x49, 0xff & PROXTH_CH0_CLS },
    { 0x4a, 0xff & (PROXTH_CH0_CLS >> 8)},
    { 0x4b, 0xff & PROXTH_CH1_CLS },
    { 0x4c, 0xff & (PROXTH_CH1_CLS >> 8)},
    { 0x4d, 0xff & PROXTH_CH2_CLS },
    { 0x4e, 0xff & (PROXTH_CH2_CLS >> 8) },
    { 0x4f, 0xff & PROXTH_CH3_CLS },
    { 0x50, 0xff & (PROXTH_CH3_CLS >> 8) },

    { 0x51, 0xff & PROXTH_CH0_FAR },
    { 0x52, 0xff & (PROXTH_CH0_FAR >> 8) },
    { 0x53, 0xff & PROXTH_CH1_FAR },
    { 0x54, 0xff & (PROXTH_CH1_FAR >> 8) },
    { 0x55, 0xff & PROXTH_CH2_FAR },
    { 0x56, 0xff & (PROXTH_CH2_FAR >> 8) },
    { 0x57, 0xff & PROXTH_CH3_FAR },
    { 0x58, 0xff & (PROXTH_CH3_FAR >> 8) },

    { 0x65, 0x00 },// 中断使能寄存器
    { 0x66, 0x00 },
    { 0x67, 0x07 },// 中断配置寄存器0
    { 0x68, 0x01 },// 中端配置寄存器1
};

#else

struct RM1X01_REG_CFG rm1002_defaultcfg[] = {

};
#endif

//RM1002B系列连续写寄存器函数；建议一次性写寄存器不超过10个
static bool rm1002_reg_write(uint8_t reg_start_addr, uint8_t *reg_array, uint8_t num, uint8_t rm1002b_addr)
{
//    if (HAL_I2C_Master_Transmit(&I2cHandle, reg_start_addr, (uint8_t *)reg_array, num, 5000) != HAL_OK)
    if (HAL_I2C_Mem_Write(&I2cHandle, (uint16_t)rm1002b_addr<<1, reg_start_addr, 1, (uint8_t *)reg_array, num, 5000) != HAL_OK)
    {
        return false;
    }
    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
//    printf("enter main3\r\n");

    return true;
}

//RM1002B系列连续读寄存器；建议一次性读寄存器不超过3个
static bool rm1002_reg_read(uint8_t reg_start_addr, uint8_t *reg_array, uint8_t num, uint8_t rm1002b_addr)
{
    
//    if (HAL_I2C_Master_Receive(&I2cHandle, reg_start_addr, (uint8_t *)reg_array, num, 5000) != HAL_OK)
    if (HAL_I2C_Mem_Read(&I2cHandle, (uint16_t)rm1002b_addr<<1, reg_start_addr, 1, (uint8_t *)reg_array, num, 5000) != HAL_OK)
    {
        return false;
    }
    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
    return true;
}


static int32_t sign24To32(uint32_t bit24)
{
    if ((bit24 & 0x800000) == 0x800000) {
        bit24 |= 0xff000000;
    }

    return bit24;
}

static int32_t sign16To32(uint32_t bit16)
{
    if ((bit16 & 0x8000) == 0x8000) {
        bit16 |= 0xffff0000;
    }

    return bit16;
}

static int32_t get_h8bit(int32_t data)
{
    int32_t ret = 0;
    ret = data;
    if (ret < 0) {
        return 0;
    }

    ret = ret >> 16;
    return ret;
}

static int32_t get_h16bit(int32_t data)
{
    int32_t ret = 0;
    ret = data;
    if (ret < 0) {
        return 0;
    }
    ret = ret >> 8;
    return ret;
}

uint32_t rm1002_adc_data_get(int dataType, int channel,uint8_t rm1002b_addr)
{
    uint8_t rx_buf[3] = {0};
    uint8_t reg_base_addr;
    uint32_t ret_data = 0;

    switch (dataType)
    {
    case RAW_DATA:
        reg_base_addr = channel * 3 + RM1X01_RAW0_CH0;
        break;
    case USE_DATA:
        reg_base_addr = channel * 3 + RM1X01_USE0_CH0;
        break;
    case AVG_DATA:
        reg_base_addr = channel * 3 + RM1X01_AVG0_CH0;
        break;
    case DIF_DATA:
        reg_base_addr = channel * 3 + RM1X01_DIF0_CH0;
        break;
    default:
        break;
    }

//  uint32_t axirq = csi_irq_save();  // 屏蔽中断
    if(rm1002_reg_read(reg_base_addr, rx_buf, 3,rm1002b_addr) == true)
    {
        ret_data = rx_buf[0] | (rx_buf[1] << 8) | (rx_buf[2] << 16);
    }

    return ret_data;
}

static void rm1002_setconfig(uint8_t rm1002b_addr)
{
    int i = 0;
    uint8_t tx_buf[4] = {0};

    for (i = 0; i < sizeof(rm1002_defaultcfg) / sizeof(rm1002_defaultcfg[0]); i++) {
        tx_buf[0] = rm1002_defaultcfg[i].reg_value;
        rm1002_reg_write(rm1002_defaultcfg[i].reg_address, tx_buf, 1, rm1002b_addr);
    }

    for (i = 0; i <= 0x68; i++) {
        if (rm1002_reg_read(i, tx_buf, 1 , rm1002b_addr) == false) {
            printf("read reg error\n");
        }
        // printf("reg: 0x%02x=[%02x]\n", i, tx_buf[0]);
    }
}


void rm1002_init(uint8_t rm1002_addr)
{
    uint8_t rxbuf[4] = {0};
    uint8_t wdata[4] = { 0x80, 0x80, 0x80, 0x80 };
    uint8_t data = 0x00;

    /* I2C initialization */
    I2cHandle.Instance             = I2C;                       /* I2C */
    I2cHandle.Init.ClockSpeed      = I2C_SPEEDCLOCK;            /* I2C communication speed */
    I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE;             /* I2C duty cycle */
    I2cHandle.Init.OwnAddress1     = rm1002_addr<<1;            /* I2C address */
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;   /* Prohibit broadcast calls */
    I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;     /* Allow clock extension */
    if (HAL_I2C_Init(&I2cHandle) != HAL_OK)
    {
        APP_ErrorHandler();
    }
    
    data = 0xcc;            //RM1002B复位
    rm1002_reg_write(0x6f, &data, 1, rm1002_addr);
    HAL_Delay(10);
    
    if(rm1002_addr == DEFAULE_I2C_ADDR_0)     //RM1002B复位后地址都是0x2b，写入地址
    {
        data = 0x03;        //11   2b
        rm1002_reg_write(0x59, &data, 1, 0x2b);
    }
    if(rm1002_addr == DEFAULE_I2C_ADDR_1)
    {
        data = 0x02;
        rm1002_reg_write(0x59, &data, 1, 0x2b);
    }
    if(rm1002_addr == DEFAULE_I2C_ADDR_2)
    {
        data = 0x01;
        rm1002_reg_write(0x59, &data, 1, 0x2b);
    }
    if(rm1002_addr == DEFAULE_I2C_ADDR_3)
    {
        data = 0x00;
        rm1002_reg_write(0x59, &data, 1, 0x2b);
    }
    HAL_Delay(10);
    
        if (rm1002_reg_read(0x70, rxbuf, 1, rm1002_addr) == false) {     //读取who am I 地址
            printf("read id error\r\n");
        }
        // printf("id: [%02x]\r\n",  rxbuf[0]);
    
    data = 0x00;
    rm1002_reg_write(0x17, &data, 1, rm1002_addr);
    HAL_Delay(70);
    rm1002_setconfig(rm1002_addr);
    HAL_Delay(10);

    data = 0xff;
    rm1002_reg_write(0x2f, &data, 1,rm1002_addr);
    HAL_Delay(800);
    
    HAL_Delay(10);

    rm1002_reg_write(0x39, wdata, 4,rm1002_addr);
}

volatile int16_t                          aADCxConvertedData;
volatile int16_t                          aTEMPERATURE;

// ADC回调
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *adchandle)
{
    aADCxConvertedData = adchandle->Instance->DR;

    aTEMPERATURE =(int16_t)((85-30)*(aADCxConvertedData-TScal1)/(TScal2-TScal1) + TStem1);
    printf("Temperature = %d \r\n", aTEMPERATURE);
}

void rm1002_intr_handler(uint8_t rm1002_addr)
{
    uint8_t streg = 0;
    uint8_t sta0, sta1, sta2, sta3;
    printf("%x, %x, %x, %x\n", sta0, sta1, sta2, sta3); // 消除warning

    rm1002_reg_read(0X74, &streg, 1,rm1002_addr);
    sta0 = !!(streg & 0x01);
    sta1 = !!(streg & 0x02);
    sta2 = !!(streg & 0x04);
    sta3 = !!(streg & 0x08);
}

void rm1002_timer_handler(uint32_t ms)
{

}

void rm1002_irq_handler(uint8_t rm1002_addr)
{
//  uart_printf("%s %d\n", __func__, __LINE__);
    rm1002_intr_handler(rm1002_addr);
//  rm1002_debug_func();
}

/*****************************************************************************************************/
// 以下是调试代码
/*****************************************************************************************************/

#if 1
float* rm1002_debug_func(uint8_t rm1002_addr, float *cxxx0, float *cxxx1, float *cxxx2, float *cxxx3)
{
    int i = 0;
    int32_t rawdata[4] = {0};
//  int32_t usedata[4] = {0};
    int32_t difdata[4] = {0};
    int32_t cxdata[4] = {0};
    uint8_t streg = {0};
    uint8_t cxoff[4] = {0};
    uint8_t stdata[4] = {0};
//  uint8_t print_index = 1;
    uint8_t rx_buf[4] = {0};
    
    static float c1[4] = {0.0};  // 传感器值
    float c2[4]        = {0.0};  // 与传感器串联电容值
    float cxx[4]       = {0};    // 引脚寄生电容
    float cx0[4]       = {0.0};  //
    float cx2[4]       = {0.0};  // 1002测出总电容值
    float coff[4]      = {0};    // pF  芯片内部减去的电容值
    
    float kill_war = cx2[0];// 消除warning
    float kill_war1 = rx_buf[0] + kill_war;
    float kill_war2 = stdata[0];
    float kill_war3 = streg;
    float kill_war4 = cxdata[0];
    float kill_war5 = difdata[0];
    
    c2[0] = 43.0;  // pF
    c2[1] = 43.0;  // pF
    c2[2] = 43.0;  // pF
    c2[3] = 43.0;  // pF

    // 当前硬件测量模型: 传感器与外部设置电容串联，再与引脚寄生电容并联,如下
    //                C1     C2
    //    CDC --------||----||----------- GND
    //             |            |
    //             |-----||-----|
    //                   Cx
    
    // 参考电容大小为5pF, ADC 为23bit
    //
    // 引脚寄生电容值每个芯片，每个通道都不通
    // 寄生电容获取方式：将传感器短路，然后获取cx2的值为c2与cxx的并联值
    if(rm1002_addr==DEFAULE_I2C_ADDR_0)
    {
        cxx[0] = cxxx0[0] - c2[0];  // 通道0 引脚寄生电容
        cxx[1] = cxxx0[1] - c2[1];  // 通道1 引脚寄生电容
        cxx[2] = cxxx0[2] - c2[2];  // 通道2 引脚寄生电容
        cxx[3] = cxxx0[3] - c2[3];  // 通道3 引脚寄生电容
    }
    if(rm1002_addr==DEFAULE_I2C_ADDR_1)
    {
        cxx[0] = cxxx1[0] - c2[0];  // 通道0 引脚寄生电容
        cxx[1] = cxxx1[1] - c2[1];  // 通道1 引脚寄生电容
        cxx[2] = cxxx1[2] - c2[2];  // 通道2 引脚寄生电容
        cxx[3] = cxxx1[3] - c2[3];  // 通道3 引脚寄生电容
    }
    if(rm1002_addr==DEFAULE_I2C_ADDR_2)
    {
        cxx[0] = cxxx2[0] - c2[0];  // 通道0 引脚寄生电容
        cxx[1] = cxxx2[1] - c2[1];  // 通道1 引脚寄生电容
        cxx[2] = cxxx2[2] - c2[2];  // 通道2 引脚寄生电容
        cxx[3] = cxxx2[3] - c2[3];  // 通道3 引脚寄生电容
    }
    if(rm1002_addr==DEFAULE_I2C_ADDR_3)
    {
        cxx[0] = cxxx3[0] - c2[0];  // 通道0 引脚寄生电容
        cxx[1] = cxxx3[1] - c2[1];  // 通道1 引脚寄生电容
        cxx[2] = cxxx3[2] - c2[2];  // 通道2 引脚寄生电容
        cxx[3] = cxxx3[3] - c2[3];  // 通道3 引脚寄生电容
    }
    
    rawdata[0] = sign24To32(rm1002_adc_data_get(RAW_DATA, 0, rm1002_addr));
    rawdata[1] = sign24To32(rm1002_adc_data_get(RAW_DATA, 1, rm1002_addr));
    rawdata[2] = sign24To32(rm1002_adc_data_get(RAW_DATA, 2, rm1002_addr));
    rawdata[3] = sign24To32(rm1002_adc_data_get(RAW_DATA, 3, rm1002_addr));

// 调整补偿电容，防止按压上限
    if (rawdata[0] > 8300000 || rawdata[0] < 20000)
    {
        uint8_t data = 0x1f;                           // RM1002B的λ0x1f通道0 0x2f通道1 0x4f通道2 0x8f通道3 0xff通道4
        rm1002_reg_write(0x2f, &data, 1, rm1002_addr); // 校准

        uint8_t data0 = 0;
        while (!(data0 & 0x01)) // 通道0 通道1 通道2 通道3
        {
            rm1002_reg_read(0x73, &data0, 1, rm1002_addr); // 等待校准完成
        }
    }
    if (rawdata[1] > 8300000 || rawdata[1] < 20000)
    {
        uint8_t data = 0x2f;                           // RM1002B的λ
        rm1002_reg_write(0x2f, &data, 1, rm1002_addr); // 校准

        uint8_t data1 = 0;
        while (!(data1 & 0x02))
        {
            rm1002_reg_read(0x73, &data1, 1, rm1002_addr); // 等待校准完成
        }
    }
    if (rawdata[2] > 8300000 || rawdata[2] < 20000)
    {
        uint8_t data = 0x4f;
        rm1002_reg_write(0x2f, &data, 1, rm1002_addr); // 校准

        uint8_t data2 = 0;
        while (!(data2 & 0x04))
        {
            rm1002_reg_read(0x73, &data2, 1, rm1002_addr); // 等待校准完成
        }
    }
    if (rawdata[3] > 8300000 || rawdata[3] < 20000)
    {
        uint8_t data = 0x8f;
        rm1002_reg_write(0x2f, &data, 1, rm1002_addr); // 校准

        uint8_t data3 = 0;
        while (!(data3 & 0x08))
        {
            rm1002_reg_read(0x73, &data3, 1, rm1002_addr); // 等待校准完成
        }
    }
    
    rm1002_reg_read(0x41, cxoff, 4, rm1002_addr);
    
    for (i = 0; i < 4; i++) {
        coff[i] = (256.0 - cxoff[i]) * 0.2;// 修改coff的值增大量程
        cx0[i] = rawdata[i] * 5.0;
        cx0[i] = cx0[i] / 8388608.0;
        cx0[i] = cx0[i] + coff[i];
        cx2[i] = cx0[i];
        cx0[i] = cx0[i] - cxx[i];
        c1[i] = (c2[i] * cx0[i]) / (c2[i] - cx0[i]);
        cxdata[i] = c1[i] * 100;
    }
#if FLEX_FLAG
    if(rm1002_addr==DEFAULE_I2C_ADDR_0)         //左边
    {
        printf("float cxxx0[4] = {%f, %f ,%f, %f};\r\n",cx2[0], cx2[1], cx2[2], cx2[3]);        // 寄生电容
    }
    if(rm1002_addr==DEFAULE_I2C_ADDR_1)         //左边
    {
        printf("float cxxx1[4] = {%f, %f ,%f, %f};\r\n",cx2[0], cx2[1], cx2[2], cx2[3]);        // 寄生电容
    }
    if(rm1002_addr==DEFAULE_I2C_ADDR_2)         //左边
    {
        printf("float cxxx2[4] = {%f, %f ,%f, %f};\r\n",cx2[0], cx2[1], cx2[2], cx2[3]);        // 寄生电容
    }
    if(rm1002_addr==DEFAULE_I2C_ADDR_3)         //左边
    {
        printf("float cxxx3[4] = {%f, %f ,%f, %f};\r\n",cx2[0], cx2[1], cx2[2], cx2[3]);        // 寄生电容
    }
#endif
    return c1;
}
#endif

// 用于消除烦人的warning
void kill_warning(void)
{
    get_h16bit(1);
    get_h8bit(1);
    sign16To32(1);
}


/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/

#endif  // RM1002_C_PRESS_TEST
