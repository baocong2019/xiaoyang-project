#include "bsp_key.h"
#include "delay.h"

u16 longkey = 0;
u8 keybuf = _KEYNULL;
u16 key_handle = _OFF;
u32 Test_Key_Time = 0;

u16 longkey2 = 0;
u8 keybuf2 = _KEYNULL2;
u16 key_handle2 = _OFF2;


void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY3_GPIO_CLK|KEY4_GPIO_CLK,ENABLE);//

	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN;
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);
}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )
	{
		/*�ȴ������ͷ� */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
		return 	KEY_ON;
	}
	else
		return KEY_OFF;
}

void scan_key(void)
{
    unsigned char key_status;

    if((longkey == 0) && (keybuf != _KEYNULL))
			{			        // ��keybuf�����Ϊ������̰������ǰ����Ѿ��ɿ���������ѭ������һ�κ󣬼�ʱ������״̬���Ϊ�ް������¡�
        keybuf = _KEYNULL;
			}

    if(key_handle == _ON)
			{ 																																//5ms����һ��
        key_handle = _OFF;    																															// ��λ5ms��־λ
        key_status = GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
        if(key_status == 0)
					{   																														//���°���
            longkey++;
					}
        else
					{  																																						// �ɿ�����
            if((longkey >= 3) && (longkey <= KEY_long_time) )
						{    																		// 15ms - 1s
							keybuf = _SHORTKEY;
						}
						else if(longkey > KEY_long_time)
						{ 																											// 1s����
							keybuf = _LONGKEY;
						}
						else
						{
							keybuf = _KEYNULL;																													// ��Ϊ�Ŷ�������״̬Ҳ��Ϊ�ް�������
						}
            longkey = 0;
					}
    }
}

void scan_key2(void)
{
    unsigned char key_status;

    if((longkey2 == 0) && (keybuf2 != _KEYNULL2))
			{			        // ��keybuf�����Ϊ������̰������ǰ����Ѿ��ɿ���������ѭ������һ�κ󣬼�ʱ������״̬���Ϊ�ް������¡�
        keybuf2 = _KEYNULL2;
			}

    if(key_handle2 == _ON2)
			{ 																																//5ms����һ��
        key_handle2 = _OFF2;    																															// ��λ5ms��־λ
        key_status = GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
        if(key_status == 0)
					{   																														//���°���
            longkey2++;
					}
        else
					{  																																						// �ɿ�����
            if((longkey2 >= 3) && (longkey2 <= KEY_long_time) )
						{    																		// 15ms - 1s
							keybuf2 = _SHORTKEY2;
						}
						else if(longkey2 > KEY_long_time)
						{ 																											// 1s����
							keybuf2 = _LONGKEY2;
						}
						else
						{
							keybuf2 = _KEYNULL2;																													// ��Ϊ�Ŷ�������״̬Ҳ��Ϊ�ް�������
						}
            longkey2 = 0;
					}
    }
}
/*********************************************END OF FILE**********************/
