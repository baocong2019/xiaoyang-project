#ifndef __Tongyong_Tim_H
#define __Tongyong_Tim_H

#include "stm32f10x.h"
/*
STM32֮��ʱ��ʱ�����
1.����ϵͳʱ�ӽ��м��㣬������72Mhz
2.��ʱ�����ʱ��=��1+ʱ�ӷ�Ƶ��.TIM_Prescaler = psc-1����/72MHZ*��1+��װ��ֵ��.TIM_Period = arr-1����
3.��������ĳ�ֵ��usΪ��λ��
*/

#define            TONGYONG_TIM                   TIM2
#define            TONGYONG_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            TONGYONG_TIM_CLK               RCC_APB1Periph_TIM2
#define            TONGYONG_TIM_PERIOD            (10-1)						//Ŀǰ����Ϊ1000us��1ms		Ƶ��0.1hz
#define            TONGYONG_TIM_PSC               (7200-1)
//#define            TONGYONG_TIM_PULSE             4

#define            TONGYONG_TIM_IRQ               TIM2_IRQn
#define            TONGYONG_TIM_IRQHandler        TIM2_IRQHandler

void TIM2_Init(void);

#endif
