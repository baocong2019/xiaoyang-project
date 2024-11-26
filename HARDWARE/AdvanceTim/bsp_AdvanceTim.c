#include "bsp_AdvanceTim.h" 
#include "pid.h"
#include "gizwits_product.h"

void TIM1_SetDutyCycle(uint16_t dutyCycle)
{
    TIM_SetCompare1(TIM1, dutyCycle);
}

void TIM1_PWM_PA8_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);             

//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); 


	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);//时钟使能

	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =ADVANCE_TIM_PSC; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //重复计数器
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 500; //设置待装入捕获比较寄存器的脉冲值
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStructure中指定的参数初始化TIM1
	TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设
//	TIM_ITConfig(TIM1,TIM_IT_Update ,ENABLE);
//	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 

	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
	// NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	// NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	// NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设						 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		 pid.C10ms++;
		 PID_out(); //输出PID运算结果到负载
		}
}

void TIM1_UP_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
  {
    // 清除中断标志位
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
  }
}

void TIM1_CC_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
  {
    // 清除中断标志位
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
  }
}


/*********************************************END OF FILE**********************/
