#include "DHT11.h"
#include "usart2.h"	 
#include "delay.h"


//DHT11数据结构体定义
DHT11_Data_TypeDef DHT11_Data;

typedef enum {true = 1, false = !true} bool;
/*----------------------------------------------------------*/
/*函数名：DHT11输出模式																			*/
/*参  数：无																								*/
/*返回值：无											  												*/											   		
/*----------------------------------------------------------*/
void DHT11_OutputMode()			//PB11
{
	GPIO_InitTypeDef DHT11_InitType;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	DHT11_InitType.GPIO_Pin = DHT11_PIN;
	DHT11_InitType.GPIO_Mode=GPIO_Mode_Out_PP;			//推挽输出
	DHT11_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(DHT11_PORT,&DHT11_InitType);
}
/*----------------------------------------------------------*/
/*函数名：DHT11输入模式																			*/
/*参  数：无																								*/
/*返回值：无											  												*/
/*----------------------------------------------------------*/
void DHT11_InputMode()
{
	GPIO_InitTypeDef DHT11_InitType;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	DHT11_InitType.GPIO_Pin = DHT11_PIN;
	DHT11_InitType.GPIO_Mode=GPIO_Mode_IN_FLOATING;			//浮空输入
	DHT11_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(DHT11_PORT,&DHT11_InitType);
}

/*----------------------------------------------------------*/
/*函数名：DHT11复位																					*/
/*参  数：无																								*/
/*返回值：无											  												*/
/*功能：复位，发送起始信号																	*/
/*----------------------------------------------------------*/
void DHT11_Rst()
{
	DHT11_OutputMode();	//输出模式
	DHT11_SetLow;
	delay_us(18*1000);	//主机拉低至少18ms
	DHT11_SetHigh;
	delay_us(30);				//主机拉高20-40us
}

/*----------------------------------------------------------*/
/*函数名：DHT11获取应答																			*/
/*参  数：无																								*/
/*返回值：1：存在，0:不存在				  												*/
/*功能：复位后，检测DHT11是否存在														*/
/*----------------------------------------------------------*/
bool DHT11_GetAck()
{
	u8 retry=0;
	
	DHT11_InputMode();	//输入模式
	
//	while(DHT11_GetStatus == 1);		//此时数据线为高，等待被从机拉低
//	while(DHT11_GetStatus == 0);		//此时数据线已被从机拉低，从机将持续40-50us低电平，后拉高

    while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return false;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return false;	    
	
	return true;
}


/*----------------------------------------------------------*/
/*函数名：DHT11读取bit																			*/
/*参  数：无																								*/
/*返回值：0低，1高，2异常	  																*/
/*----------------------------------------------------------*/
u8 DHT11_ReadBit()
{
	u32 temp = 0;		//计时
	//一个字节从低电平开始
	while(DHT11_GetStatus == 1)
	{
		delay_us(1);
		temp++;
		if(temp>2000)			//200us仍没被拉低，返回异常
			return 2;
	}
	temp = 0;
	//等待变为高电平，正常应为12-14us（不论数据是0还是1）
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
/*函数名：DHT11读取字节																			*/
/*参  数：无																								*/
/*返回值：读取到的字节 																			*/
/*说明：暂未对BIT的异常进行处理															*/
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
/*函数名：DHT11读取数据																			*/
/*参  数：无																								*/
/*返回值：1：成功，0：失败  																*/
/*----------------------------------------------------------*/
u8 DHT11_ReadData()
{
	DHT11_Rst();
	
	if(DHT11_GetAck() == 0)
	{
//		printf("未收到应答");
		return 0;
	}

	DHT11_Data.HumiInt = DHT11_ReadByte();
	DHT11_Data.HumiDec = DHT11_ReadByte();
	DHT11_Data.TempInt = DHT11_ReadByte();
	DHT11_Data.TempDec = DHT11_ReadByte();
	DHT11_Data.Check = DHT11_ReadByte();
	

	if(DHT11_Data.Check == DHT11_Data.HumiInt+DHT11_Data.HumiDec+DHT11_Data.TempInt+DHT11_Data.TempDec)
	{
//		printf("成功测得温湿度！\r\n");
//		printf("温度：%d.%d",DHT11_Data.TempInt,DHT11_Data.TempDec);
//		printf("湿度：%d.%d\r\n\r\n",DHT11_Data.HumiInt,DHT11_Data.HumiDec);
		return 1;		//读取成功
	}
	else
	{
//		printf("校验失败");
		return 0;
	}

	
}

