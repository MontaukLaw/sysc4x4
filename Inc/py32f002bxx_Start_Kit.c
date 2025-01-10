/**
  ******************************************************************************
  * @file    py32f002bxx_Start_Kit.c
  * @author  MCU Application Team
  * @brief   This file provides set of firmware functions to manage Leds, 
  *          push-button available on Start Kit.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "py32f002bxx_Start_Kit.h"

/**
  * @brief PY32F002Bxx STK BSP Driver version number
  */
#define __PY32F002Bxx_STK_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define __PY32F002Bxx_STK_BSP_VERSION_SUB1   (0x00U) /*!< [23:16] sub1 version */
#define __PY32F002Bxx_STK_BSP_VERSION_SUB2   (0x00U) /*!< [15:8]  sub2 version */
#define __PY32F002Bxx_STK_BSP_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define __PY32F002Bxx_STK_BSP_VERSION        ((__PY32F002Bxx_STK_BSP_VERSION_MAIN << 24) \
                                            |(__PY32F002Bxx_STK_BSP_VERSION_SUB1 << 16) \
                                            |(__PY32F002Bxx_STK_BSP_VERSION_SUB2 << 8 ) \
                                            |(__PY32F002Bxx_STK_BSP_VERSION_RC))

#ifdef HAL_UART_MODULE_ENABLED
UART_HandleTypeDef DebugUartHandle;
#endif
//GPIO_TypeDef* LED_PORT[LEDn] = {LED3_GPIO_PORT};
//const uint16_t LED_PIN[LEDn] = {LED3_PIN};
//GPIO_TypeDef* LED_PORT[LEDn] = {LED_G_GPIO_PORT,LED1_R_GPIO_PORT,LED1_G_GPIO_PORT,LED1_B_GPIO_PORT,LED2_R_GPIO_PORT,LED2_G_GPIO_PORT,LED2_B_GPIO_PORT,LED_OFF_GPIO_PORT};
//const uint16_t LED_PIN[LEDn] = {LED_G_PIN,LED1_R_PIN,LED1_G_PIN,LED1_B_PIN,LED2_R_PIN,LED2_G_PIN,LED2_B_PIN,LED_OFF_PIN};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {USER_BUTTON_GPIO_PORT };
const uint16_t BUTTON_PIN[BUTTONn] = {USER_BUTTON_PIN };
const uint8_t BUTTON_IRQn[BUTTONn] = {USER_BUTTON_EXTI_IRQn };

/** @addtogroup PY32F002Bxx_STK_Exported_Functions
  * @{
  */

/**
  * @brief  This method returns the PY32F002B STK BSP Driver revision.
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __PY32F002Bxx_STK_BSP_VERSION;
}

/** @addtogroup LED_Functions
  * @{
  */

/**
  * @brief  Configures LED GPIO.
  * @param  Led Specifies the Led to be configured.
  *         This parameter can be one of following parameters:
  *         @arg  LED3
  * @retval None
  */
#if 0
void BSP_LED_Init()
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the GPIO_LED Clock */
  LEDA_GPIO_CLK_ENABLE();
  LEDB_GPIO_CLK_ENABLE();
  LEDC_GPIO_CLK_ENABLE();


  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN_7 ;                    //LED_G     PA7
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
    
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 ;          //LED1_RGB     PB1,PB0,PB2
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;                        //LED2_RGB     PB7,PB5,PC1
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5 | GPIO_PIN_7, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
}
#endif
void RM1002_RST_Init()
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the GPIO_LED Clock */
  LEDA_GPIO_CLK_ENABLE();
  LEDB_GPIO_CLK_ENABLE();
  LEDC_GPIO_CLK_ENABLE();
    
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
  
//  GPIO_InitStruct.Pin = GPIO_PIN_0;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
}
/**
  * @brief  DeInitialize LED GPIO.
  * @param  Led Specifies the Led to be deconfigured.
  *         This parameter can be one of the following values:
  *         @arg  LED3
  * @note BSP_LED_DeInit() does not disable the GPIO clock
  * @retval None
  */
#if 0
void BSP_LED_DeInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Turn off LED */
//  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
  /* DeInit the GPIO_LED pin */
  GPIO_InitStruct.Pin = LED_PIN[Led];
  HAL_GPIO_DeInit(LED_PORT[Led], GPIO_InitStruct.Pin);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *   @arg  LED3
  * @retval None
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}
void BSP_RM1002B_1_On()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
}

//void BSP_RM1002B_2_On()
//{
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
//}

/**
  * @brief  Turns selected LED Off.
  * @param  Led Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *            @arg  LED3
  * @retval None
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}
void BSP_RM1002B_1_Off()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
}
//void BSP_RM1002B_2_Off()
//{
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//}

/**
  * @brief  Toggles the selected LED.
  * @param  Led Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *            @arg  LED3
  * @retval None
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}
#endif
//void BSP_LED_Toggle(Led_TypeDef Led)
//{
//  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
//}
/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_USER
  * @param  ButtonMode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg  BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability
  * @retval None
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef gpioinitstruct;

  /* Enable the BUTTON Clock */
  BUTTONx_GPIO_CLK_ENABLE(Button);

  gpioinitstruct.Pin = BUTTON_PIN[Button];
  gpioinitstruct.Pull = GPIO_NOPULL;
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

  if(ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpioinitstruct.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);
  }

  if(ButtonMode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    gpioinitstruct.Mode   = GPIO_MODE_IT_FALLING;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}

/**
  * @brief  Push Button DeInit.
  * @param  Button: Button to be configured
  *   This parameter should be: BUTTON_USER
  * @note PB DeInit does not disable the GPIO clock
  * @retval None
  */
void BSP_PB_DeInit(Button_TypeDef Button)
{
  GPIO_InitTypeDef gpio_init_structure;

  gpio_init_structure.Pin = BUTTON_PIN[Button];
  HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_init_structure.Pin);
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_USER
  * @retval Button state.
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

#ifdef HAL_UART_MODULE_ENABLED
/**
  * @brief  DEBUG_USART GPIO Config,Mode Config,115200 8-N-1
  * @param  None
  * @retval None
  */
void BSP_USART_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  DEBUG_USART_CLK_ENABLE();

  DebugUartHandle.Instance          = DEBUG_USART;

  DebugUartHandle.Init.BaudRate     = DEBUG_USART_BAUDRATE;
  DebugUartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  DebugUartHandle.Init.StopBits     = UART_STOPBITS_1;
  DebugUartHandle.Init.Parity       = UART_PARITY_NONE;
  DebugUartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  DebugUartHandle.Init.Mode         = UART_MODE_TX_RX;

  HAL_UART_Init(&DebugUartHandle);

  DEBUG_USART_RX_GPIO_CLK_ENABLE();
  DEBUG_USART_TX_GPIO_CLK_ENABLE();

  /**USART GPIO Configuration
    PB4     ------> USART1_TX
    PB5     ------> USART1_RX
    */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;
  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
  GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;

  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);

  /* ENABLE NVIC */
  HAL_NVIC_SetPriority(DEBUG_USART_IRQ,0,1);
  HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ );
}

//#if (defined (__CC_ARM)) || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
///**
//  * @brief  writes a character to the usart
//  * @param  ch
//  *         *f
//  * @retval the character
//  */
//int fputc(int ch, FILE *f)
//{
//  /* Send a byte to USART */
//  HAL_UART_Transmit(&DebugUartHandle, (uint8_t *)&ch, 1, 1000);

//  return (ch);
//}

///**
//  * @brief  get a character from the usart
//  * @param  *f
//  * @retval a character
//  */
//int fgetc(FILE *f)
//{
//  int ch;
//  HAL_UART_Receive(&DebugUartHandle, (uint8_t *)&ch, 1, 1000);
//  return (ch);
//}

#elif defined(__ICCARM__)
/**
  * @brief  writes a character to the usart
  * @param  ch
  *         *f
  * @retval the character
  */
int putchar(int ch)
{
  /* Send a byte to USART */
  HAL_UART_Transmit(&DebugUartHandle, (uint8_t *)&ch, 1, 1000);

  return (ch);
}
#elif  defined(__GNUC__)
/**
  * @brief  writes a character to the usart
  * @param  ch
  * @retval the character
  */
int __io_putchar(int ch)
{
  /* Send a byte to USART */
  HAL_UART_Transmit(&DebugUartHandle, (uint8_t *)&ch, 1, 1000);

  return ch;
}

int _write(int file, char *ptr, int len)
{
  int DataIdx;
  for (DataIdx=0;DataIdx<len;DataIdx++)
  {
    __io_putchar(*ptr++);
  }
  return len;
}
#endif

//#endif
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
