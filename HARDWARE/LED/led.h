#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED0 PBout(12)// PB12
// #define LED1 PEout(5)// PE5
#define BEEP PAout(12) // PA12

#define LED0_on LED0=0;
#define LED0_off LED0=1;
// #define LED1_on LED1=0;
// #define LED1_off LED1=1;

//#define BEEP_on BEEP=1;
//#define BEEP_off BEEP=0;

#define BEEP_on BEEP=1;
#define BEEP_off BEEP=0;

#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态

#define LED0_TOGGLE		  digitalToggle(GPIOB,GPIO_Pin_12);

#define BEEP_TOGGLE		  digitalToggle(GPIOA,GPIO_Pin_12);

// #define LED1_TOGGLE		  digitalToggle(GPIOE,GPIO_Pin_5);

void LED_Init(void);//初始化
void BEEP_Init(void);
void Yuyin_io_init(void);
void IR_Init(void);

#endif
