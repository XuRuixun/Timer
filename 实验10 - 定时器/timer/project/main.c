/****************************************Copyright (c)****************************************************
**                                        
**                                      [����ķ�Ƽ�]
**                                        IIKMSIK
**--------------File Info---------------------------------------------------------------------------------
** File name:			     main.c
** Last modified Date:         
** Last Version:		   
** Descriptions:		   ʹ�õ�SDK�汾-SDK_11.0.0
**						
**--------------------------------------------------------------------------------------------------------
** Created by:			FiYu
** Created date:		2016-5-19
** Version:			    1.0
** Descriptions:		��ʱ��ʵ��
**--------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf_drv_timer.h"
#include "bsp.h"
#include "app_error.h"
#include "nrf.h"
#include "boards.h"

#define BEEP    12


/* ��������LEDռ�õ�nRF51822�ܽ���Դ
P0.21�����������ָʾ��D1
P0.22�����������ָʾ��D2
P0.23�����������ָʾ��D3
P0.24�����������ָʾ��D4

��Ҫ�̽Ӷ�Ӧ������ñ
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
 * ��  �� : main����
 * ��  �� : ��
 * ����ֵ : ��
 ***********************************************************************************************/ 
int main(void)
{
		nrf_gpio_cfg_output(BEEP);  //����P0.12Ϊ���
		nrf_gpio_pin_clear(BEEP);   //��������ʼ״̬Ϊ������
    uint32_t time_ms = 1000; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;
	  
    LEDS_CONFIGURE(LEDS_MASK);//���ÿ�����������LED�Ĺܽ�Ϊ���
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
        __WFI();//����˯�ߣ��ȴ��ж�
    }
}
/********************************************END FILE*******************************************/
