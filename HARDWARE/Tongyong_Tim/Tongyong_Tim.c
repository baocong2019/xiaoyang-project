#include "Tongyong_Tim.h"
#include "bsp_key.h"
#include "led.h"
#include "gizwits_product.h"

unsigned char Caluator;
unsigned int  TIM2_time;
unsigned int flag_500ms;
unsigned char genxin_temp;
unsigned char time_1s_get_flag;
bool time_10s_get_flag;

extern u16 key_handle;
extern u16 key_handle2;

static void TONGYONG_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TONGYONG_TIM_IRQ ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void TONGYONG_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TONGYONG_TIM_APBxClock_FUN(TONGYONG_TIM_CLK,ENABLE);
	TIM_TimeBaseStructure.TIM_Period=TONGYONG_TIM_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler= TONGYONG_TIM_PSC;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TONGYONG_TIM, &TIM_TimeBaseStructure);
	TIM_ITConfig(TONGYONG_TIM,TIM_IT_Update,ENABLE);
	TIM_Cmd(TONGYONG_TIM, ENABLE);
}

void TIM2_Init(void)
{
	TONGYONG_TIM_NVIC_Config();
	TONGYONG_TIM_Mode_Config();
}

void  TONGYONG_TIM_IRQHandler (void)//1ms 进入中断一次 tim2生成
{

	if ( TIM_GetITStatus( TONGYONG_TIM, TIM_IT_Update) != RESET )
	{
		Caluator++;
		TIM2_time++;
		flag_500ms++;

		TIM_ClearITPendingBit(TONGYONG_TIM , TIM_IT_Update);
		gizTimerMs();

		if( Caluator == 5 ) // 5 * 1000 us =5ms
			{
				Caluator = 0;
				key_handle = _ON;
				key_handle2 = _ON2;
			}

		if( flag_500ms == 500 )
			{
				flag_500ms = 0;
				genxin_temp=1;
			}

		if(TIM2_time == 1000 )
		{
			TIM2_time = 0;
			time_1s_get_flag += 1;
			if(time_1s_get_flag == 10)
			{
				time_1s_get_flag = 0;
				time_10s_get_flag = 1;
			}
			LED0_TOGGLE
		}
	}
}
