#ifndef __Tongyong_Tim_H
#define __Tongyong_Tim_H

#include "stm32f10x.h"
/*
STM32之定时器时间计算
1.根据系统时钟进行计算，假设是72Mhz
2.定时器溢出时间=（1+时钟分频（.TIM_Prescaler = psc-1））/72MHZ*（1+重装载值（.TIM_Period = arr-1））
3.计算出来的初值以us为单位。
*/

#define            TONGYONG_TIM                   TIM2
#define            TONGYONG_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            TONGYONG_TIM_CLK               RCC_APB1Periph_TIM2
#define            TONGYONG_TIM_PERIOD            (10-1)						//目前周期为1000us即1ms		频率0.1hz
#define            TONGYONG_TIM_PSC               (7200-1)
//#define            TONGYONG_TIM_PULSE             4

#define            TONGYONG_TIM_IRQ               TIM2_IRQn
#define            TONGYONG_TIM_IRQHandler        TIM2_IRQHandler

void TIM2_Init(void);

#endif
