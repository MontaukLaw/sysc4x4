#include "led.h"

// LED初始化
void APP_LEDInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct = {0};
    
    /* 时钟使能 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /* PA0 PA1 PA5 PA6 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;            /* 输出模式 */
    GPIO_InitStruct.Pull = GPIO_PULLUP;                    /* 使能上拉 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* 高速模式 */
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);                /* 使能 */
    
    /* PB0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


//配置系统时钟 HSI 24Mhz
void System_Clock_Config_HSI_24Mhz(void)
{
  /* OSC 配置 */
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;            //配置类型 HSI 
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                              //打开HSI                      
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                              //HSI不分频             
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz;     //HSI输入 24Mhz   

  HAL_RCC_OscConfig(&RCC_OscInitStruct);//根据配置初始化OSC

  /* 系统时钟配置 */
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | 
                                RCC_CLOCKTYPE_HCLK   | 
                                RCC_CLOCKTYPE_PCLK1; 

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;      //系统时钟源 ---->HSI 24Mhz                                     
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;          //AHB总线 不分频 24MHZ         
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;           //APB总线 AHB 2分频 12Mhz              

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);   //Flash 0等待周期
}




