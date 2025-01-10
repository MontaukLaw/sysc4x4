#ifndef __ARITHMETIC_H
#define __ARITHMETIC_H

#include "py32f0xx_hal.h"

// �ṹ�����Ͷ���
typedef struct 
{
    float LastP;    //�ϴι���Э���� ��ʼ��ֵΪ0.02
    float Now_P;    //��ǰ����Э���� ��ʼ��ֵΪ0
    float out;      //�������˲������ ��ʼ��ֵΪ0
    float Kg;       //���������� ��ʼ��ֵΪ0
    float Q;        //��������Э���� ��ʼ��ֵΪ0.001
    float R;        //�۲�����Э���� ��ʼ��ֵΪ0.543
}KFP;               //Kalman Filter parameter

float averageFilter(float in_data, int channel);
float weightedAverageFilter(float in_data, int channel);
float middleFilter(float in_data, int channel);
float low_pass_filter(float x, float prev_y, float alpha);
float high_pass_filter(float x, float prev_x, float prev_y, float alpha);
float filter1(float in_data, int channel);
float kalmanFilter(KFP *kfp, float input);

extern volatile float pre_low_value[4] ;
extern volatile float pre_high_value[4];
extern volatile float pre_high_sig[4];
extern KFP KFP_value;
    
#endif 

