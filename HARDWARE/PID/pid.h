#ifndef _pid_
#define _pid_
#include "sys.h"
typedef struct
{
	float Sv;//�û��趨ֵ
	float Pv;//ʵ��ֵ

	float Kp;//����ϵ��
	float T;  //PID��������--��������
	float Ti;//����ʱ�䳣��
	float Td; //΢��ʱ�䳣��
	float x;	
	
	float Ek;  //����ƫ��
	float Ek_1;//�ϴ�ƫ��
	float SEk; //��ʷƫ��֮��
	
	float Iout;//�������
	float Pout;//�������
	float Dout;//΢�����
	
	float OUT0;//һ��ά�ֵ����ֵ����ֹʧ��

	float OUT;//PID���
	
	
	u16 C10ms;

	u16 pwmcycle;//pwm����

	u32 times;
	
}PID;

extern PID pid; //���PID�㷨����Ҫ������

void PID_Calc(void); //pid����
void PID_Init(void);
void PID_Calc_overshoot(void);  //pid����-���崦��
void PID_out(void);
#endif








