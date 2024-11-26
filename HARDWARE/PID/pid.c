#include "pid.h"

PID pid;
u16 pw;

float out;
float ki;

//void PID_Init()
//{
//  pid.Sv=0;
//	pid.Kp=35;
//	pid.T=480;
//  pid.Ti=3500000;
//	pid.Td=900;
//	pid.pwmcycle=200;
//	pid.OUT0=1;
//}

void PID_Init()//PID数值的初始化，KP\TI\TD的数值都是需要根据 不同情况自己试出来的，改变单一变量，进行设置更改。不合适，自动进行微调
{
	pid.Sv=0;
	pid.Kp=35;
	pid.T=480;
	pid.Ti=3500000;
	pid.Td=900;
	pid.pwmcycle=200;
	pid.OUT0=1;
}

void PID_Calc()  //pid计算     每一次的动态调整周期过后，进行当前值与实际值的差异计算，从而进行数值改变。
{
	float DelEk;//本次和上次的差值，最近一次的差值
	float ti;

	float td;
	float kd;

	if(pid.C10ms<(pid.T))//最小计算周期未到
	{
		return ;
	}

	pid.x=pid.Pv-pid.Sv;//得到当前的偏差值
	pid.Ek=pid.x;
	pid.Pout=pid.Kp*pid.Ek;  //p 比例输出
	pid.SEk+=pid.Ek;//历史偏差总和
	DelEk=pid.Ek-pid.Ek_1;//最近两次偏差之差
	ti=pid.T/pid.Ti;
	ki=ti*pid.Kp;
	pid.Iout=ki*pid.SEk*pid.Kp; //i
	td=pid.Td/pid.T;
	kd=pid.Kp*td;
	pid.Dout=kd*DelEk;    //d 微分输出
	out= pid.Pout+ pid.Iout+ pid.Dout;

/*判断算出的数是否符合控制要求*/
	if(out>pid.pwmcycle)//不能比最大值还大
	{
		pid.OUT=pid.pwmcycle;  //max
	}
	else if(out<0)//不能比最小值还小
	{
  		pid.OUT=pid.OUT0;   //min
	}
	else
	{
		pid.OUT=out;
	}
		pid.Ek_1=pid.Ek;//更新偏差
		pid.C10ms=0;
}

void PID_out()  //输出PID运算结果到负载---每1ms被调用1次        改变负载使用的是PA8管脚控制占空比
{
	pw++;
	if(pw>=pid.pwmcycle)
	{
	pw=0;
	}

	if(pw<pid.OUT)
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	}
	else
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	}

}







