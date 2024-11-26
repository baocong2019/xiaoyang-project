#ifndef __DHT22_H
#define __DHT22_H 
#include "sys.h"  
#include "delay.h"
 
/************************************************************
**> File Name: 			dht22.c																	
**> Description:		DHT22/AM2302 GPIO - hardware driver
**> Author: 				xinso
**> blog:  					xinso.blog.csdn.net
**> Created Time: 	2021.04.06
**--------------------------------------------------------
**> 本程序只供学习使用，未经作者许可，不得用于其它任何用途
*************************************************************/
 
/* DHT22 I/O引脚定义 */
#define RCC_GPIO_AM2302   RCC_APB2Periph_GPIOE
#define GPIO_AM2302       GPIOE
#define PIN_AM2302        GPIO_Pin_13
 
#define	DHT22_DQ_OUT 			PEout(13) 		//PG11_out  
#define	DHT22_DQ_IN  			PEin(13)   		//PG11_in
 
/* DHT22 I/O口方向设置 */
#define DHT22_IO_IN()  {GPIOE->CRH&=0xFFFF0FFF;GPIOE->CRH|=4<<12;}	//PE13_in		 Floating 	input
#define DHT22_IO_OUT() {GPIOE->CRH&=0xFFFF0FFF;GPIOE->CRH|=3<<12;}	//PE13_out	 Pull-Push output
			
 
extern u8 DHT22_Init(void); 			//Init DHT22
extern u8 DHT22_Read_Data(float *temperature,float *humidity); //Read DHT22 Value
static u8 DHT22_Read_Byte(void);	//Read One Byte
static u8 DHT22_Read_Bit(void);	//Read One Bit
static u8 DHT22_Check(void);	//Chack DHT22
static void DHT22_Rst(void);	//Reset DHT22 
 
#endif
