#ifndef _DHT11_H_
#define _DHT11_H_

#include "stm32f10x.h"

//typedef enum {true = 1, false = !true} bool;


#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_Pin_11

#define DHT11_SetHigh		GPIO_SetBits(DHT11_PORT,DHT11_PIN)
#define DHT11_SetLow		GPIO_ResetBits(DHT11_PORT,DHT11_PIN)

#define DHT11_GetStatus 	GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)

#define	DHT11_DQ_OUT PBout(11) //���ݶ˿�	PA0 
#define	DHT11_DQ_IN  PBin(11)  //���ݶ˿�	PA0 

#define Humi_Threshold 	90
#define Temp_Threshold 	29
typedef struct
{
	u8 TempInt;			//�¶���������
	
	u8 TempDec;			//�¶�С�����֣�һλ��
	
	u8 HumiInt;			//ʪ����������
			
	u8 HumiDec;			//ʪ��С�����֣�һλ��
	
	u8 Check;				//У��λ
}DHT11_Data_TypeDef;

extern DHT11_Data_TypeDef DHT11_Data;

void DHT11_OutputMode(void);
void DHT11_InputMode(void);
void DHT11_Rst(void);
//bool DHT11_GetAck(void);
u8 DHT11_ReadBit(void);
u8 DHT11_ReadByte(void);
u8 DHT11_ReadData(void);

#endif


