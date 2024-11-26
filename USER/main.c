/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-11-17
 * XXX
 * 2024年11月17日 2024年11月17日21:03:59
 * 目前完成功能：
 * 1.LED可控
 * 2.定时器可用
 * 3.LCD点亮并显示
 * 4.按键支持长短按识别
 * 5.温湿度传感器获取成功（目前只有整数位）
 * 6.串口1 串口2 使用
 * 7.机智云平台连接成功
 * 8.机智云平台数据上传成功
 * 9.PWM档位风扇调节控制 0-9挡 （0关机 1-9档位） 调节档位自动切换到手动模式
 * @copyright Copyright (c) 2024
 *
 */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "QDTFT_Demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "DHT22.h"
#include "bsp_key.h"
#include "Tongyong_Tim.h"
#include "usart2.h"
#include <stdbool.h>
#include "bsp_AdvanceTim.h"
#include "gizwits_product.h"//wifi头文件
#include "pid.h"
#include "common.h"//机智云通用包含文件

extern u8 keybuf;//按键key1的变量值
extern u8 keybuf2;//按键key2的变量值
extern unsigned char wifi_state;//wifi是否连接到云服务
extern unsigned char time_1s_get_flag;//检测1s是否到了标志位
extern unsigned char genxin_temp;//PID周期更新标志位
extern bool time_10s_get_flag;//检测10s是否到了标志位

void Key_scan_WIFI(void);
void Read_hum_tem_display_lcd(void);
void Dis_Beep_sta_and_wifi_sta(void);
void Dis_wifi_time_dengyu_local_time(void);
void yuyin_judge(void);
void Shou_ZiDong_display_lcd(void);
void Read_IR_sta(void);

u8 Goal_TEM=15;//目标温度
float TEM=0;//实测温度
float HUM=0;//实测湿度
u8 Key_num=0;//按键数值

u8 fan_speed_dangwei=Fan_speed;//风速档位
bool shou_zidong_swtich=ZiDONG;//手自动开关  手动：0  自动：1
bool time_get_swtich=ON;//时间获取开关  定时关闭：0  定时开启：1

u16 ret = 0;//dht22检测是否有返回值
u8 data[5] = {0};//温湿度数据返回数值存储到数组里

unsigned char displayNum[15]; // 用于存放转换后的字符串
bool beep_sta=0;//蜂鸣器状态
u8 wifi_Time_shi=0,wifi_time_fen=0;//wifi获取的时间变量
u8 local_hour=10,local_minute=10; //本地设置的时间变量

int main()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	SystemInit();
	delay_init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	BEEP_Init();
	KEY_GPIO_Config();
	TIM2_Init();
	TIM1_PWM_PA8_Init();			//pwm初始化管道PA8
	TIM3_Int_Init(10-1,7200-1);
	PID_Init();						//PID参数初始化
	uart1_init(9600);
	uart2_init(9600);	
	DHT22_Init();					//温湿度传感器初始化
//	USART2_SendString("usart2 is ok\r\n");//printf用于串口2
	userInit();						//机智云移植步骤需添加函数
	gizwitsInit();				//机智云平台初始化2	
	Lcd_Init();	
	LCD_LED_SET;
	Lcd_Clear(GRAY0);
	Yuyin_io_init();
	IR_Init();
                    //延时300ms
	GPIO_SetBits(GPIOA,GPIO_Pin_12);   //BEEP引脚拉高， 等同BEEP=1;
	delay_ms(500); 	
	GPIO_ResetBits(GPIOA,GPIO_Pin_12); //BEEP引脚拉低， 等同BEEP=0;
	delay_ms(500);
	
	TIM1_SetDutyCycle(0);//上电前风扇状态关闭
	while(1)
	{
		yuyin_judge();//实时接受语音模块指令
		
		 if(shou_zidong_swtich==ZiDONG)
		 {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			if(genxin_temp==1)
			{
				genxin_temp=0;
				PID_Calc(); //每到周期时间是，进行pid的动态控制，即重新设计占空比例，调节定时器3的占空比
				pid.Sv=Goal_TEM;//把当前的目标温度进行作为新的目标值计算，计算差异
				pid.Pv=TEM;
				TIM_Cmd(TIM3, ENABLE);
			}
		 }

		 if(shou_zidong_swtich==ShouDONG)
		 {
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //GPIOF9
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用功能
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_Init(GPIOA,&GPIO_InitStructure);   
		 }	
		 
		userHandle();
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//wifi 必加函数体 上传数据用

		Key_scan_WIFI();

		if(time_1s_get_flag==1)//每1s获取一次时间
		{
			time_1s_get_flag=0;
			Read_hum_tem_display_lcd();//读取目的温度数据以及温湿度传感器数据，并显示到LCD上
			Dis_Beep_sta_and_wifi_sta();//显示蜂鸣器状态以及wifi连接状态以及wifi时间显示
			Dis_wifi_time_dengyu_local_time();//显示wifi时间是否等于本地时间
			Shou_ZiDong_display_lcd();//显示手自动档位
			Read_IR_sta();

			gizwitsGetNTP();
			if((wifi_Time_shi!=0)&&(wifi_time_fen!=0))
			{
				wifi_state=Ture;
			}
			sprintf((char *)displayNum, "w_time:%d:%d",wifi_Time_shi,wifi_time_fen);
			Gui_DrawFont_GBK16(0, 100, BLUE, GRAY0, displayNum);
		}
	}
}

void Dis_wifi_time_dengyu_local_time()
{
	if(time_get_swtich==ON)
	{	
		sprintf((char *)displayNum, "Time:%2d:%2d",local_hour,local_minute);
		Gui_DrawFont_GBK16(0, 80, BLUE, GRAY0, displayNum);
	
		if((wifi_Time_shi==local_hour)&&(wifi_time_fen==local_minute))
		{
			BEEP_on
			delay_ms(1000);
			BEEP_off
		
			shou_zidong_swtich=ShouDONG;
			TIM1_SetDutyCycle(999);//开启风扇

			sprintf((char *)displayNum, "time_get");
			Gui_DrawFont_GBK16(0, 20, BLUE, GRAY0, displayNum);
			delay_ms(500);
			sprintf((char *)displayNum, "            ");
			Gui_DrawFont_GBK16(0, 20, BLUE, GRAY0, displayNum);
		}		
	}
	else
	{
		sprintf((char *)displayNum, "Time:--:--");
		Gui_DrawFont_GBK16(0, 80, BLUE, GRAY0, displayNum);
	}
}

void Dis_Beep_sta_and_wifi_sta()
{
	if(wifi_state == Ture)
	{
		sprintf((char *)displayNum, "WIFI:Connect");
		Gui_DrawFont_GBK16(0, 140, BLUE, GRAY0, displayNum);
	}
	else
	{
		sprintf((char *)displayNum, "WIFI:Not Con");
		Gui_DrawFont_GBK16(0, 140, BLUE, GRAY0, displayNum);
	}
}

void Key_scan_WIFI()
{
		static unsigned char i =0;

		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
		{
			while (Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == 0);
			Goal_TEM+=1;
			sprintf((char *)displayNum, "G_TEM:%dC",Goal_TEM);
			Gui_DrawFont_GBK16(0, 40, BLUE, GRAY0, displayNum);
		}

		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN)==KEY_ON)
		{
			while (Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == 0);
			Goal_TEM-=1;
			sprintf((char *)displayNum, "G_TEM:%dC",Goal_TEM);
			Gui_DrawFont_GBK16(0, 40, BLUE, GRAY0, displayNum);
		}

		if(Key_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN)==KEY_ON)
		{
					while (Key_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN) == 0);
					i++;
					if(i%2==1)
					{
//						USART2_SendString("into WIFI_AIRLINK_MODE\r\n");
						sprintf((char *)displayNum, "AirLink Mode");
						Gui_DrawFont_GBK16(0, 30, BLUE, GRAY0, displayNum);
						delay_ms(500);
						sprintf((char *)displayNum, "            ");
						Gui_DrawFont_GBK16(0, 30, BLUE, GRAY0, displayNum);
						gizwitsSetMode(WIFI_AIRLINK_MODE);
					}
					else
					{
	//					USART2_SendString("into soft dot_MODE\r\n");
						sprintf((char *)displayNum, "softdot Mode");
						Gui_DrawFont_GBK16(0, 30, BLUE, GRAY0, displayNum);
						delay_ms(500);
						sprintf((char *)displayNum, "            ");
						Gui_DrawFont_GBK16(0, 30, BLUE, GRAY0, displayNum);
						gizwitsSetMode(WIFI_SOFTAP_MODE);
					}
		}

		if(Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN)==KEY_ON)
		{
					while (Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == 0);
	//			USART2_SendString("into WIFI_RESET_MODE\r\n");
					sprintf((char *)displayNum, "SoftReset Mode");
					Gui_DrawFont_GBK16(0, 30, BLUE, GRAY0, displayNum);
					delay_ms(500);
					sprintf((char *)displayNum, "              ");
					Gui_DrawFont_GBK16(0, 30, BLUE, GRAY0, displayNum);
					gizwitsSetMode(WIFI_RESET_MODE);
		}
}

void Read_hum_tem_display_lcd()
{
	sprintf((char *)displayNum, "G_TEM:%dC",Goal_TEM);
	Gui_DrawFont_GBK16(0, 40, BLUE, GRAY0, displayNum);
	
	DHT22_Read_Data(&TEM, &HUM);
	sprintf((char *)displayNum, "T:%.1fC H:%.1f%%",TEM,HUM);
	Gui_DrawFont_GBK16(0, 60, BLUE, GRAY0, displayNum);

	if(Goal_TEM>=TEM)//温度高于目标温度，打开蜂鸣器
	{
		BEEP_on
	}
}

void yuyin_judge()
{
	if(Key_Scan(GPIOF,GPIO_Pin_0)==KEY_ON)
	{
		while (Key_Scan(GPIOF,GPIO_Pin_0) == 0);
		shou_zidong_swtich=ShouDONG;
		fan_speed_dangwei=9;
		TIM1_SetDutyCycle(999);//开启风扇
	}
	if(Key_Scan(GPIOF,GPIO_Pin_1)==KEY_ON)
	{
		while (Key_Scan(GPIOF,GPIO_Pin_1) == 0);
		shou_zidong_swtich=ShouDONG;
		fan_speed_dangwei=0;
		TIM1_SetDutyCycle(0);//关闭风扇
	}
	if(Key_Scan(GPIOF,GPIO_Pin_2)==KEY_ON)
	{
		while (Key_Scan(GPIOF,GPIO_Pin_1) == 0);
		shou_zidong_swtich=ShouDONG;
		fan_speed_dangwei++;
		TIM1_SetDutyCycle(fan_speed_dangwei*100);//增大风扇
	}
	if(Key_Scan(GPIOF,GPIO_Pin_3)==KEY_ON)
	{
		while (Key_Scan(GPIOF,GPIO_Pin_1) == 0);
		shou_zidong_swtich=ShouDONG;
		fan_speed_dangwei--;
		TIM1_SetDutyCycle(fan_speed_dangwei*100);//减小风扇
	}
}

void Shou_ZiDong_display_lcd()
{
	if(shou_zidong_swtich==ZiDONG)//自动档位下，不显示风扇档位，依据温度传感器进行自动调节
	{
		sprintf((char *)displayNum, "AUTO");
		Gui_DrawFont_GBK16(0, 0, BLUE, GRAY0, displayNum);
		
		sprintf((char *)displayNum, "      ");
		Gui_DrawFont_GBK16(70, 0, BLUE, GRAY0, displayNum);
	}
	else//手动情况下，才可以设置风扇档位，以及接受语音播报
	{
		sprintf((char *)displayNum, "MANU");
		Gui_DrawFont_GBK16(0, 0, BLUE, GRAY0, displayNum);
		
		sprintf((char *)displayNum, "fan:%2d",fan_speed_dangwei);
		Gui_DrawFont_GBK16(70, 0, BLUE, GRAY0, displayNum);
	}	
}

void Read_IR_sta()
{

    if(	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==KEY_ON)
	{
		sprintf((char *)displayNum, "IR:Low ");
		Gui_DrawFont_GBK16(0, 120, BLUE, GRAY0, displayNum);
	}
    else
	{
		sprintf((char *)displayNum, "IR:High");
		Gui_DrawFont_GBK16(0, 120, BLUE, GRAY0, displayNum);
	}
}
