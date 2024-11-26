#ifndef __KEY_H
#define	__KEY_H


#include "stm32f10x.h"

#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA
#define    KEY1_GPIO_PIN	 GPIO_Pin_1

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY2_GPIO_PORT    GPIOB
#define    KEY2_GPIO_PIN		 GPIO_Pin_6

#define    KEY3_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY3_GPIO_PORT    GPIOB
#define    KEY3_GPIO_PIN		 GPIO_Pin_5

#define    KEY4_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY4_GPIO_PORT    GPIOA
#define    KEY4_GPIO_PIN		 GPIO_Pin_4


#define    KEY_long_time     40

 /** 按键按下标置宏
	*  按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	*  若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	0
#define KEY_OFF	1
#define KEY_LONG 2

#define _ON 1
#define _OFF 0
#define _KEYNULL 0
#define _SHORTKEY 1
#define _LONGKEY 2

#define _ON2 1
#define _OFF2 0
#define _KEYNULL2 0
#define _SHORTKEY2 1
#define _LONGKEY2 2

void KEY_GPIO_Config(void);
void scan_key(void);
void scan_key2(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);


#endif /* __KEY_H */

