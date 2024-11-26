/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "sys.h"
#include "bsp_AdvanceTim.h" 

unsigned char wifi_state=0;//wifi是否连接到云服务
static uint32_t timerMsCount;

/** Current datapoint */
dataPoint_t currentDataPoint;

extern u8 Goal_TEM;
extern u8 local_hour;
extern u8 local_minute;
extern float TEM;
extern float HUM;
extern u8 wifi_Time_shi;
extern u8 wifi_time_fen;
extern u8 fan_speed_dangwei;
extern bool shou_zidong_swtich;
extern bool time_get_swtich;

// extern u8 local_hour;
// extern u8 local_minute;
/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
    moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
    protocolTime_t *ptime = (protocolTime_t *)gizdata;
    
#if MODULE_TYPE
    gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
    moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

    if((NULL == info) || (NULL == gizdata))
    {
        return -1;
    }

    for(i=0; i<info->num; i++)
    {
        switch(info->event[i])
        {
        case EVENT_Switch:
            currentDataPoint.valueSwitch = dataPointPtr->valueSwitch;
            GIZWITS_LOG("Evt: EVENT_Switch %d \n", currentDataPoint.valueSwitch);
            if(0x01 == currentDataPoint.valueSwitch)
            {
            //user handle
            shou_zidong_swtich=currentDataPoint.valueSwitch;  
            }
            else
            {
            shou_zidong_swtich=currentDataPoint.valueSwitch;
            }
            break;
        case EVENT_Switchtime:
            currentDataPoint.valueSwitchtime = dataPointPtr->valueSwitchtime;
            GIZWITS_LOG("Evt: EVENT_Switchtime %d \n", currentDataPoint.valueSwitchtime);
            if(0x01 == currentDataPoint.valueSwitchtime)
            {
            //user handle
            time_get_swtich=ON;
            }
            else
            {
            //user handle    
            time_get_swtich=OFF;
            }
            break;


        case EVENT_G_temp_net:
            currentDataPoint.valueG_temp_net = dataPointPtr->valueG_temp_net;
            GIZWITS_LOG("Evt:EVENT_G_temp_net %d\n",currentDataPoint.valueG_temp_net);
            //user handle
                Goal_TEM=currentDataPoint.valueG_temp_net;
            break;
        case EVENT_fanspeed:
            currentDataPoint.valuefanspeed = dataPointPtr->valuefanspeed;
            GIZWITS_LOG("Evt:EVENT_fanspeed %d\n",currentDataPoint.valuefanspeed);
            //user handle
                if(shou_zidong_swtich==ShouDONG)
                {
                    fan_speed_dangwei=currentDataPoint.valuefanspeed;
                    TIM1_SetDutyCycle(fan_speed_dangwei*100);
                }
                else{fan_speed_dangwei=0;}
            break;
        case EVENT_Hour:
            currentDataPoint.valueHour = dataPointPtr->valueHour;
            GIZWITS_LOG("Evt:EVENT_Hour %d\n",currentDataPoint.valueHour);
            //user handle
                local_hour=currentDataPoint.valueHour;
            break;
        case EVENT_Mint:
            currentDataPoint.valueMint = dataPointPtr->valueMint;
            GIZWITS_LOG("Evt:EVENT_Mint %d\n",currentDataPoint.valueMint);
            //user handle
                local_minute=currentDataPoint.valueMint;
            break;


        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
				wifi_state=1;
            break;
        case WIFI_DISCON_ROUTER:
 
            break;
        case WIFI_CON_M2M:
 
            break;
        case WIFI_DISCON_M2M:
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            //user handle , Fetch data from [data] , size is [len]
            break;
        case WIFI_NTP:
            GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
            wifi_Time_shi=ptime->hour;
            wifi_time_fen=ptime->minute;
            
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
#if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",gprsInfoData->Type);
#else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
#endif
        break;
        default:
            break;
        }
    }

    return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
 /*
    currentDataPoint.valuetemp = ;//Add Sensor Data Collection
    currentDataPoint.valuehumi = ;//Add Sensor Data Collection

    */
    currentDataPoint.valueSwitch = shou_zidong_swtich;
    currentDataPoint.valueSwitchtime = time_get_swtich;
//    currentDataPoint.valueG_temp_net = Goal_TEM;
//    currentDataPoint.valuefanspeed = fan_speed_dangwei;
//    currentDataPoint.valueHour = local_hour;
//    currentDataPoint.valueMint = local_minute;
    currentDataPoint.valuetemp = (int)TEM;
    currentDataPoint.valuehumi = (int)HUM;
}


/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
    currentDataPoint.valueSwitch = ;
    currentDataPoint.valueSwitchtime = ;
    currentDataPoint.valueG_temp_net = ;
    currentDataPoint.valuefanspeed = ;
    currentDataPoint.valueHour = ;
    currentDataPoint.valueMint = ;
    currentDataPoint.valuetemp = ;
    currentDataPoint.valuehumi = ;
    */
    currentDataPoint.valueSwitch = shou_zidong_swtich;
    currentDataPoint.valueSwitchtime = time_get_swtich;
    currentDataPoint.valueG_temp_net = Goal_TEM;
    currentDataPoint.valuefanspeed = fan_speed_dangwei;
    currentDataPoint.valueHour = local_hour;
    currentDataPoint.valueMint = local_minute;
    currentDataPoint.valuetemp = (int)TEM;
    currentDataPoint.valuehumi = (int)HUM;
}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();

}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
    gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
    uint8_t value = 0;
    gizPutData(&value, 1);
}


 /**
 * @brief uartWrite

 * Serial write operation, send data to the WiFi module

 * @param buf      : Data address
 * @param len       : Data length
 *
 * @return : Not 0,Serial send success;
 *           -1，Input Param Illegal
 */
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;

    if(NULL == buf)
    {
        return -1;
    }

    #ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif

    for(i=0; i<len; i++)
    {
        USART_SendData(USART1, buf[i]);
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
        if(i >=2 && buf[i] == 0xFF)
        {
            USART_SendData(USART1, 0x55);
          //USART_SendData(USART1, buf[i]);  
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
        }
    }   
    return len;
}


