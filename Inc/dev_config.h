#ifndef __DEV_CONFIG_H__
#define __DEV_CONFIG_H__

#ifndef NULL
#define NULL ((void *)0)
#endif


#define RM1002_C_PRESS_TEST     1
#define RM1002_TOUCH            0
#define SWTIMER                 1


#if RM1002_TOUCH
#include "./rm1002/rm1002.h"
#define RM1002_INIT
#define RM1002_DEBUG
#endif

#if RM1002_C_PRESS_TEST
#include "rm1002.h"
#define RM1002_INIT
#define RM1002_DEBUG
//#define RM1002_INTR     // 中断
//#define RM1002_TIMER    // 定时器
#endif


#define DEFAULE_I2C_ADDR_0  (0x2B) 
#define DEFAULE_I2C_ADDR_1  (0x2A) 
#define DEFAULE_I2C_ADDR_2  (0x29) 
#define DEFAULE_I2C_ADDR_3  (0x28) 



#endif  //__DEV_CONFIG_H__


