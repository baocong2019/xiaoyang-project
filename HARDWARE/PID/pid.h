#ifndef _pid_
#define _pid_
#include "sys.h"
typedef struct
{
	float Sv;//用户设定值
	float Pv;//实际值

	float Kp;//比例系数
	float T;  //PID计算周期--采样周期
	float Ti;//积分时间常数
	float Td; //微分时间常数
	float x;	
	
	float Ek;  //本次偏差
	float Ek_1;//上次偏差
	float SEk; //历史偏差之和
	
	float Iout;//积分输出
	float Pout;//比例输出
	float Dout;//微分输出
	
	float OUT0;//一个维持的输出值，防止失控

	float OUT;//PID输出
	
	
	u16 C10ms;

	u16 pwmcycle;//pwm周期

	u32 times;
	
}PID;

extern PID pid; //存放PID算法所需要的数据

void PID_Calc(void); //pid计算
void PID_Init(void);
void PID_Calc_overshoot(void);  //pid计算-过冲处理
void PID_out(void);
#endif








