/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-11-17
 * XXX
 * 2024��11��17�� 2024��11��17��21:03:59
 * Ŀǰ��ɹ��ܣ�
 * 1.LED�ɿ�
 * 2.��ʱ������
 * 3.LCD��������ʾ
 * 4.����֧�ֳ��̰�ʶ��
 * 5.��ʪ�ȴ�������ȡ�ɹ���Ŀǰֻ������λ��
 * 6.����1 ����2 ʹ��
 * 7.������ƽ̨���ӳɹ�
 * 8.������ƽ̨�����ϴ��ɹ�
 * 9.PWM��λ���ȵ��ڿ��� 0-9�� ��0�ػ� 1-9��λ�� ���ڵ�λ�Զ��л����ֶ�ģʽ
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
#include "gizwits_product.h"//wifiͷ�ļ�
#include "pid.h"
#include "common.h"//������ͨ�ð����ļ�

extern u8 keybuf;//����key1�ı���ֵ
extern u8 keybuf2;//����key2�ı���ֵ
extern unsigned char wifi_state;//wifi�Ƿ����ӵ��Ʒ���
extern unsigned char time_1s_get_flag;//���1s�Ƿ��˱�־λ
extern unsigned char genxin_temp;//PID���ڸ��±�־λ
extern bool time_10s_get_flag;//���10s�Ƿ��˱�־λ

void Key_scan_WIFI(void);
void Read_hum_tem_display_lcd(void);
void Dis_Beep_sta_and_wifi_sta(void);
void Dis_wifi_time_dengyu_local_time(void);
void yuyin_judge(void);
void Shou_ZiDong_display_lcd(void);
void Read_IR_sta(void);

u8 Goal_TEM=15;//Ŀ���¶�
float TEM=0;//ʵ���¶�
float HUM=0;//ʵ��ʪ��
u8 Key_num=0;//������ֵ

u8 fan_speed_dangwei=Fan_speed;//���ٵ�λ
bool shou_zidong_swtich=ZiDONG;//���Զ�����  �ֶ���0  �Զ���1
bool time_get_swtich=ON;//ʱ���ȡ����  ��ʱ�رգ�0  ��ʱ������1

u16 ret = 0;//dht22����Ƿ��з���ֵ
u8 data[5] = {0};//��ʪ�����ݷ�����ֵ�洢��������

unsigned char displayNum[15]; // ���ڴ��ת������ַ���
bool beep_sta=0;//������״̬
u8 wifi_Time_shi=0,wifi_time_fen=0;//wifi��ȡ��ʱ�����
u8 local_hour=10,local_minute=10; //�������õ�ʱ�����

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
	TIM1_PWM_PA8_Init();			//pwm��ʼ���ܵ�PA8
	TIM3_Int_Init(10-1,7200-1);
	PID_Init();						//PID������ʼ��
	uart1_init(9600);
	uart2_init(9600);	
	DHT22_Init();					//��ʪ�ȴ�������ʼ��
//	USART2_SendString("usart2 is ok\r\n");//printf���ڴ���2
	userInit();						//��������ֲ��������Ӻ���
	gizwitsInit();				//������ƽ̨��ʼ��2	
	Lcd_Init();	
	LCD_LED_SET;
	Lcd_Clear(GRAY0);
	Yuyin_io_init();
	IR_Init();
                    //��ʱ300ms
	GPIO_SetBits(GPIOA,GPIO_Pin_12);   //BEEP�������ߣ� ��ͬBEEP=1;
	delay_ms(500); 	
	GPIO_ResetBits(GPIOA,GPIO_Pin_12); //BEEP�������ͣ� ��ͬBEEP=0;
	delay_ms(500);
	
	TIM1_SetDutyCycle(0);//�ϵ�ǰ����״̬�ر�
	while(1)
	{
		yuyin_judge();//ʵʱ��������ģ��ָ��
		
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
				PID_Calc(); //ÿ������ʱ���ǣ�����pid�Ķ�̬���ƣ����������ռ�ձ��������ڶ�ʱ��3��ռ�ձ�
				pid.Sv=Goal_TEM;//�ѵ�ǰ��Ŀ���¶Ƚ�����Ϊ�µ�Ŀ��ֵ���㣬�������
				pid.Pv=TEM;
				TIM_Cmd(TIM3, ENABLE);
			}
		 }

		 if(shou_zidong_swtich==ShouDONG)
		 {
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
				
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //GPIOF9
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //���ù���
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_Init(GPIOA,&GPIO_InitStructure);   
		 }	
		 
		userHandle();
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//wifi �ؼӺ����� �ϴ�������

		Key_scan_WIFI();

		if(time_1s_get_flag==1)//ÿ1s��ȡһ��ʱ��
		{
			time_1s_get_flag=0;
			Read_hum_tem_display_lcd();//��ȡĿ���¶������Լ���ʪ�ȴ��������ݣ�����ʾ��LCD��
			Dis_Beep_sta_and_wifi_sta();//��ʾ������״̬�Լ�wifi����״̬�Լ�wifiʱ����ʾ
			Dis_wifi_time_dengyu_local_time();//��ʾwifiʱ���Ƿ���ڱ���ʱ��
			Shou_ZiDong_display_lcd();//��ʾ���Զ���λ
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
			TIM1_SetDutyCycle(999);//��������

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

	if(Goal_TEM>=TEM)//�¶ȸ���Ŀ���¶ȣ��򿪷�����
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
		TIM1_SetDutyCycle(999);//��������
	}
	if(Key_Scan(GPIOF,GPIO_Pin_1)==KEY_ON)
	{
		while (Key_Scan(GPIOF,GPIO_Pin_1) == 0);
		shou_zidong_swtich=ShouDONG;
		fan_speed_dangwei=0;
		TIM1_SetDutyCycle(0);//�رշ���
	}
	if(Key_Scan(GPIOF,GPIO_Pin_2)==KEY_ON)
	{
		while (Key_Scan(GPIOF,GPIO_Pin_1) == 0);
		shou_zidong_swtich=ShouDONG;
		fan_speed_dangwei++;
		TIM1_SetDutyCycle(fan_speed_dangwei*100);//�������
	}
	if(Key_Scan(GPIOF,GPIO_Pin_3)==KEY_ON)
	{
		while (Key_Scan(GPIOF,GPIO_Pin_1) == 0);
		shou_zidong_swtich=ShouDONG;
		fan_speed_dangwei--;
		TIM1_SetDutyCycle(fan_speed_dangwei*100);//��С����
	}
}

void Shou_ZiDong_display_lcd()
{
	if(shou_zidong_swtich==ZiDONG)//�Զ���λ�£�����ʾ���ȵ�λ�������¶ȴ����������Զ�����
	{
		sprintf((char *)displayNum, "AUTO");
		Gui_DrawFont_GBK16(0, 0, BLUE, GRAY0, displayNum);
		
		sprintf((char *)displayNum, "      ");
		Gui_DrawFont_GBK16(70, 0, BLUE, GRAY0, displayNum);
	}
	else//�ֶ�����£��ſ������÷��ȵ�λ���Լ�������������
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
