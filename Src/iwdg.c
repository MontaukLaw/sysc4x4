#include "iwdg.h"

IWDG_HandleTypeDef IwdgHandle; // 看门狗句柄

void IWDG_Init(void)
{
    // 看门狗初始化配置
    IwdgHandle.Instance = IWDG;                     // 设置IWDG基地址
    IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;  // 设置IWDG预分频值为32
    IwdgHandle.Init.Reload = 10000000;              // 设置IWDG重装载值，约1小时超时
    
    if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK)       // 初始化IWDG
    {
        printf("Watchdog init failed!\r\n");
        while (1);                                  // 初始化失败时死循环
    }
}










