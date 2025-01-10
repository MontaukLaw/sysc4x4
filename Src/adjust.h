#ifndef __ADJUST_H
#define __ADJUST_H

#include "main.h"
#include "py32f0xx_hal.h"

extern volatile uint8_t G_Adjustment_Flag; // �õ�������ֻ����һ��
extern volatile uint8_t   RM1002B_X;       // RM��ַ
extern float *c_read_value;                // ����ֵ
extern volatile float c_value[16];         // ������ֵ
extern volatile uint8_t G_Printf_Flag;     // 20ms��ӡһ��
extern float cxxx0[4];                     // RM0��������ֵ
extern float cxxx1[4];                     // RM1��������ֵ
extern float cxxx2[4];                     // RM2��������ֵ
extern float cxxx3[4];                     // RM3��������ֵ

void calibrate_channel(uint8_t channel);

#endif
