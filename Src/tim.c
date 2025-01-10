#include "tim.h"

TIM_HandleTypeDef          TimHandle;  // 定时器句柄

volatile uint8_t   t_10ms_Flag            = 0;    // 10ms flag
volatile uint16_t  t_10ms_count           = 0;    // 10ms count
volatile uint32_t  hour_count             = 0;    // 一小时计数器
volatile uint8_t   hour_timeout_flag      = 0;    // 超时标志

// TIM初始化
void APP_TimConfig(void)
{
    /* TIM1 */
    TimHandle.Instance = TIM1;
    /* Period = 3000 - 1 */
    TimHandle.Init.Period            = 1 - 1;                   //1ms
    /* Prescaler = 8000 - 1 */
    TimHandle.Init.Prescaler         = 23999 - 1;
    /* ClockDivision = 0 */
    TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    /* Counter direction = Up */
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    /* Repetition = 0 */
    TimHandle.Init.RepetitionCounter = 1 - 1;
    /* Auto-reload register not buffered */
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    /* Initialize TIM1 */
    if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
    {
        APP_ErrorHandler();
    }
    
    HAL_TIM_Base_Start_IT(&TimHandle);
}

// TIM回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    t_10ms_count++;
    
    if(t_10ms_count > 9)//10ms
    {
        t_10ms_count = 0;
        t_10ms_Flag = 1;
        
        // 只有在没有正常工作时才增加计数
        if (!G_USART1_RX_Flag && !t_10ms_Flag && G_setFlag != 0x0F)
        {
            hour_count++;
            // 360000 = 1小时/10ms
            if(hour_count >= 360000)  
            {
                hour_timeout_flag = 1;
                hour_count = 0;
            }
        }
        else
        {
            // 有正常工作就重置计数器
            hour_count = 0;
            hour_timeout_flag = 0;
        }
    }
}

// 重置一小时计时器
void Reset_Hour_Timer(void)
{
    hour_count = 0;
    hour_timeout_flag = 0;
}

// 错误处理函数
void APP_ErrorHandler(void)
{
    while (1)
    {
    }
}










