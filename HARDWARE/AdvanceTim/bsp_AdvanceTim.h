#ifndef __BSP_ADVANCETIME_H
#define __BSP_ADVANCETIME_H


#include "stm32f10x.h"


/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM1

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (999-1)						//Ŀǰ����Ϊ1s
#define            ADVANCE_TIM_PSC               (71-1)
//#define            ADVANCE_TIM_PULSE             7

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM1 ����Ƚ�ͨ��
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

// TIM1 ����Ƚ�ͨ���Ļ���ͨ��
#define            ADVANCE_TIM_CH1N_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_CH1N_PORT          GPIOB
#define            ADVANCE_TIM_CH1N_PIN           GPIO_Pin_13

//// TIM1 ����Ƚ�ͨ����ɲ��ͨ��
//#define            ADVANCE_TIM_BKIN_GPIO_CLK      RCC_APB2Periph_GPIOB
//#define            ADVANCE_TIM_BKIN_PORT          GPIOB
//#define            ADVANCE_TIM_BKIN_PIN           GPIO_Pin_12


/*****************************************************************************************/
// TIM1 ����Ƚ�ͨ��CH3
#define            ADVANCE_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH3_PORT          GPIOA
#define            ADVANCE_TIM_CH3_PIN           GPIO_Pin_10
//#define            ADVANCE_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOE
//#define            ADVANCE_TIM_CH3_PORT          GPIOE
//#define            ADVANCE_TIM_CH3_PIN           GPIO_Pin_13

// TIM1 ����Ƚ�ͨ���Ļ���ͨ��CH3
#define            ADVANCE_TIM_CH3N_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_CH3N_PORT          GPIOB
#define            ADVANCE_TIM_CH3N_PIN           GPIO_Pin_15

/**************************��������********************************/

void TIM1_PWM_PA8_Init(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM1_SetDutyCycle(uint16_t dutyCycle);

#endif	/* __BSP_ADVANCETIME_H */


