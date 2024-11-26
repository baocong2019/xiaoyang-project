#include "DHT11.h"
#include "usart2.h"	 
#include "delay.h"


//DHT11���ݽṹ�嶨��
DHT11_Data_TypeDef DHT11_Data;

typedef enum {true = 1, false = !true} bool;
/*----------------------------------------------------------*/
/*��������DHT11���ģʽ																			*/
/*��  ������																								*/
/*����ֵ����											  												*/											   		
/*----------------------------------------------------------*/
void DHT11_OutputMode()			//PB11
{
	GPIO_InitTypeDef DHT11_InitType;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	DHT11_InitType.GPIO_Pin = DHT11_PIN;
	DHT11_InitType.GPIO_Mode=GPIO_Mode_Out_PP;			//�������
	DHT11_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(DHT11_PORT,&DHT11_InitType);
}
/*----------------------------------------------------------*/
/*��������DHT11����ģʽ																			*/
/*��  ������																								*/
/*����ֵ����											  												*/
/*----------------------------------------------------------*/
void DHT11_InputMode()
{
	GPIO_InitTypeDef DHT11_InitType;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	DHT11_InitType.GPIO_Pin = DHT11_PIN;
	DHT11_InitType.GPIO_Mode=GPIO_Mode_IN_FLOATING;			//��������
	DHT11_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(DHT11_PORT,&DHT11_InitType);
}

/*----------------------------------------------------------*/
/*��������DHT11��λ																					*/
/*��  ������																								*/
/*����ֵ����											  												*/
/*���ܣ���λ��������ʼ�ź�																	*/
/*----------------------------------------------------------*/
void DHT11_Rst()
{
	DHT11_OutputMode();	//���ģʽ
	DHT11_SetLow;
	delay_us(18*1000);	//������������18ms
	DHT11_SetHigh;
	delay_us(30);				//��������20-40us
}

/*----------------------------------------------------------*/
/*��������DHT11��ȡӦ��																			*/
/*��  ������																								*/
/*����ֵ��1�����ڣ�0:������				  												*/
/*���ܣ���λ�󣬼��DHT11�Ƿ����														*/
/*----------------------------------------------------------*/
bool DHT11_GetAck()
{
	u8 retry=0;
	
	DHT11_InputMode();	//����ģʽ
	
//	while(DHT11_GetStatus == 1);		//��ʱ������Ϊ�ߣ��ȴ����ӻ�����
//	while(DHT11_GetStatus == 0);		//��ʱ�������ѱ��ӻ����ͣ��ӻ�������40-50us�͵�ƽ��������

    while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return false;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return false;	    
	
	return true;
}


/*----------------------------------------------------------*/
/*��������DHT11��ȡbit																			*/
/*��  ������																								*/
/*����ֵ��0�ͣ�1�ߣ�2�쳣	  																*/
/*----------------------------------------------------------*/
u8 DHT11_ReadBit()
{
	u32 temp = 0;		//��ʱ
	//һ���ֽڴӵ͵�ƽ��ʼ
	while(DHT11_GetStatus == 1)
	{
		delay_us(1);
		temp++;
		if(temp>2000)			//200us��û�����ͣ������쳣
			return 2;
	}
	temp = 0;
	//�ȴ���Ϊ�ߵ�ƽ������ӦΪ12-14us������������0����1��
	while(DHT11_GetStatus == 0);
	delay_us(40);
	if(DHT11_GetStatus == 0)
		return 0;
	else
	{
		while(DHT11_GetStatus == 0);
		return 1;
	}
}
/*----------------------------------------------------------*/
/*��������DHT11��ȡ�ֽ�																			*/
/*��  ������																								*/
/*����ֵ����ȡ�����ֽ� 																			*/
/*˵������δ��BIT���쳣���д���															*/
/*----------------------------------------------------------*/
u8 DHT11_ReadByte()
{
	u8 Byte = 0, i = 0;
	for(i=0;i<8;i++)
	{
		Byte<<=1;
		Byte|=DHT11_ReadBit();
	}
	return Byte;
}

/*----------------------------------------------------------*/
/*��������DHT11��ȡ����																			*/
/*��  ������																								*/
/*����ֵ��1���ɹ���0��ʧ��  																*/
/*----------------------------------------------------------*/
u8 DHT11_ReadData()
{
	DHT11_Rst();
	
	if(DHT11_GetAck() == 0)
	{
//		printf("δ�յ�Ӧ��");
		return 0;
	}

	DHT11_Data.HumiInt = DHT11_ReadByte();
	DHT11_Data.HumiDec = DHT11_ReadByte();
	DHT11_Data.TempInt = DHT11_ReadByte();
	DHT11_Data.TempDec = DHT11_ReadByte();
	DHT11_Data.Check = DHT11_ReadByte();
	

	if(DHT11_Data.Check == DHT11_Data.HumiInt+DHT11_Data.HumiDec+DHT11_Data.TempInt+DHT11_Data.TempDec)
	{
//		printf("�ɹ������ʪ�ȣ�\r\n");
//		printf("�¶ȣ�%d.%d",DHT11_Data.TempInt,DHT11_Data.TempDec);
//		printf("ʪ�ȣ�%d.%d\r\n\r\n",DHT11_Data.HumiInt,DHT11_Data.HumiDec);
		return 1;		//��ȡ�ɹ�
	}
	else
	{
//		printf("У��ʧ��");
		return 0;
	}

	
}

