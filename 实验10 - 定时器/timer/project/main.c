/****************************************Copyright (c)****************************************************
**                                        
**                                      [艾克姆科技]
**                                        IIKMSIK
**--------------File Info---------------------------------------------------------------------------------
** File name:			     main.c
** Last modified Date:         
** Last Version:		   
** Descriptions:		   使用的SDK版本-SDK_11.0.0
**						
**--------------------------------------------------------------------------------------------------------
** Created by:			FiYu
** Created date:		2016-5-19
** Version:			    1.0
** Descriptions:		定时器实验
**--------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf_drv_timer.h"
#include "bsp.h"
#include "app_error.h"
#include "nrf.h"
#include "boards.h"

#define BEEP    12


/* 开发板中LED占用的nRF51822管脚资源
P0.21：输出：驱动指示灯D1
P0.22：输出：驱动指示灯D2
P0.23：输出：驱动指示灯D3
P0.24：输出：驱动指示灯D4

需要短接对应的跳线帽
*/
const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(0);

const uint8_t leds_list[LEDS_NUMBER] = LEDS_LIST;



/**
 * @brief Handler for timer events.
 */
void timer_led_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    static uint32_t i;
    uint32_t led_to_invert = (1 << leds_list[(i++) % LEDS_NUMBER]);
    
    switch(event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
					  nrf_gpio_pin_toggle(BEEP);
            LEDS_INVERT(led_to_invert);
						
            break;
        
        default:
            //Do nothing.
            break;
    }   
			//nrf_gpio_pin_toggle(BEEP);
}




/**********************************************************************************************
 * 描  述 : main函数
 * 参  数 : 无
 * 返回值 : 无
 ***********************************************************************************************/ 
int main(void)
{
		nrf_gpio_cfg_output(BEEP);  //配置P0.12为输出
		nrf_gpio_pin_clear(BEEP);   //蜂鸣器初始状态为不鸣响
    uint32_t time_ms = 1000; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;
	  
    LEDS_CONFIGURE(LEDS_MASK);//配置开发板上驱动LED的管脚为输出
    LEDS_OFF(LEDS_MASK);
	
		err_code = nrf_drv_timer_create(nrf_TIMER_MODE_SINGLE_SHOT);
    
    //Configure TIMER_LED for generating simple light effect - leds on board will invert his state one after the other.
    err_code = nrf_drv_timer_init(&TIMER_LED, NULL, timer_led_event_handler);

    APP_ERROR_CHECK(err_code);
	  
    
    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_LED, time_ms);
    
    nrf_drv_timer_extended_compare(
         &TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    
    nrf_drv_timer_enable(&TIMER_LED);
	
	
	
	

    while(1)
    {
        __WFI();//进入睡眠，等待中断
    }
}
/********************************************END FILE*******************************************/
