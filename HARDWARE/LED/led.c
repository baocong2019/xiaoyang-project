#include "led.h"

void LED_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);

	// GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 ;			//继电器接口
	// GPIO_Init(GPIOA, &GPIO_InitStructure);
	// GPIO_ResetBits(GPIOA,GPIO_Pin_8);	
}

void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能端口时钟

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 ;//蜂鸣器开关
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);	//把蜂鸣器关上
//	GPIO_SetBits(GPIOA,GPIO_Pin_12);
}

void Yuyin_io_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void IR_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
