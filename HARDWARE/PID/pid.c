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

void PID_Init()//PID��ֵ�ĳ�ʼ����KP\TI\TD����ֵ������Ҫ���� ��ͬ����Լ��Գ����ģ��ı䵥һ�������������ø��ġ������ʣ��Զ�����΢��
{
	pid.Sv=0;
	pid.Kp=35;
	pid.T=480;
	pid.Ti=3500000;
	pid.Td=900;
	pid.pwmcycle=200;
	pid.OUT0=1;
}

void PID_Calc()  //pid����     ÿһ�εĶ�̬�������ڹ��󣬽��е�ǰֵ��ʵ��ֵ�Ĳ�����㣬�Ӷ�������ֵ�ı䡣
{
	float DelEk;//���κ��ϴεĲ�ֵ�����һ�εĲ�ֵ
	float ti;

	float td;
	float kd;

	if(pid.C10ms<(pid.T))//��С��������δ��
	{
		return ;
	}

	pid.x=pid.Pv-pid.Sv;//�õ���ǰ��ƫ��ֵ
	pid.Ek=pid.x;
	pid.Pout=pid.Kp*pid.Ek;  //p �������
	pid.SEk+=pid.Ek;//��ʷƫ���ܺ�
	DelEk=pid.Ek-pid.Ek_1;//�������ƫ��֮��
	ti=pid.T/pid.Ti;
	ki=ti*pid.Kp;
	pid.Iout=ki*pid.SEk*pid.Kp; //i
	td=pid.Td/pid.T;
	kd=pid.Kp*td;
	pid.Dout=kd*DelEk;    //d ΢�����
	out= pid.Pout+ pid.Iout+ pid.Dout;

/*�ж���������Ƿ���Ͽ���Ҫ��*/
	if(out>pid.pwmcycle)//���ܱ����ֵ����
	{
		pid.OUT=pid.pwmcycle;  //max
	}
	else if(out<0)//���ܱ���Сֵ��С
	{
  		pid.OUT=pid.OUT0;   //min
	}
	else
	{
		pid.OUT=out;
	}
		pid.Ek_1=pid.Ek;//����ƫ��
		pid.C10ms=0;
}

void PID_out()  //���PID������������---ÿ1ms������1��        �ı为��ʹ�õ���PA8�ܽſ���ռ�ձ�
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







