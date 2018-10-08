/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      output.c
  * @brief:     The driver of output.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2018.09.17
  * @updata: 
  */
  
#include "output.h"
#include "drv_gpio.h"
#include "drv_timer.h"
#include "common_data.h"
#include <arm_math.h>

static rt_device_t rt_output_dev;

/**
  * @brief : ������ʼ��
  * @param : [none].
  * @return: [0] 
  * @updata: [2018-9-17][������][����]
  */ 
static int rt_hw_output_init(void)
{
    rt_output_dev = rt_device_find(RT_PIN_NAME);	
    
    if (rt_output_dev == RT_NULL)
    {
        DO_PRINTF("output pin device is not found !!!"); 
    }
    else
    {
        DO_PRINTF("output pin device is found ~~~"); 
    }
		
    return (0);
}
INIT_DEVICE_EXPORT(rt_hw_output_init);



/**
  * @brief : ��բ����
  * @param : [none].
  * @return: [none] 
  * @updata: [2018-09-17][������][����]
  */  
static rt_uint8_t rt_hw_output_close_operate(rt_uint8_t type)
{
	switch (type)
    {
		case DO_CLOSE:
             pin_status[INDEX_SWITCH_OPEN_DO].status = GPIO_PIN_SET;	
             pin_status[INDEX_SWITCH_CLOSE_DO].status = GPIO_PIN_RESET;			
             rt_device_write(rt_output_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));				
             rt_device_write(rt_output_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));	                    
             break;         
    } 
	return (0);
	
}

/**
  * @brief : ��բ����
  * @param : [none].
  * @return: [none] 
  * @updata: [2018-09-17][������][����]
  */  
static rt_uint8_t rt_hw_output_open_operate(rt_uint8_t type)
{
	switch (type)
    {
        case DO_OPEN:
             pin_status[INDEX_SWITCH_CLOSE_DO].status = GPIO_PIN_SET;	
             pin_status[INDEX_SWITCH_OPEN_DO].status = GPIO_PIN_RESET;			
             rt_device_write(rt_output_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));				
             rt_device_write(rt_output_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));	                    
             break;             
    } 
	
	return (0);
	
}
/**
  * @brief : �ֺ�բ��λ����
  * @param : [none].
  * @return: [none] 
  * @updata: [2018-09-17][������][����]
  */  
static rt_uint8_t rt_hw_output_reset_operate(rt_uint8_t type)
{
	switch (type)
    {
        case DO_CLOSE_RECOVERY:
             pin_status[INDEX_SWITCH_CLOSE_DO].status = GPIO_PIN_SET;	
             pin_status[INDEX_SWITCH_OPEN_DO].status = GPIO_PIN_SET;			
             rt_device_write(rt_output_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));				
             rt_device_write(rt_output_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));	                    
             break;
        case DO_OPEN_RECOVERY:
             pin_status[INDEX_SWITCH_CLOSE_DO].status = GPIO_PIN_SET;	
             pin_status[INDEX_SWITCH_OPEN_DO].status = GPIO_PIN_SET;			
             rt_device_write(rt_output_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DO], sizeof(struct rt_device_pin_status));				
             rt_device_write(rt_output_dev, 0, &pin_status[INDEX_SWITCH_OPEN_DO], sizeof(struct rt_device_pin_status));	                    
             break;		
    } 
	
	return (0);
	
}

/**
  * @brief : �ֺ�բ����
  * @param : [none].
  * @return: [none] 
  * @updata: [2018-09-17][������][����]
  */  
rt_uint8_t rt_hw_output_operate(rt_uint16_t addr, rt_uint8_t operate_type)
{
	rt_uint8_t rtl = 0;
	switch (addr)
	{
		case ADDR_LOGIC_ACT:			
		    break;
	    case ADDR_REMOTE_OPERATE:
		case ADDR_LOCAL_OPERATE:	
		case ADDR_HANDHELD_OPER:	
            switch (operate_type)	
			{
				case DO_CLOSE: // �ֶ���բ
					rtl = rt_hw_output_close_operate(DO_CLOSE);					
					break;
							
				case DO_OPEN:  // �ֶ���բ
					rtl = rt_hw_output_open_operate(DO_OPEN);					
					break;
				
				case DO_CLOSE_RECOVERY:
					rtl = rt_hw_output_reset_operate(DO_CLOSE_RECOVERY);
					break;
				
				case DO_OPEN_RECOVERY:
					rtl = rt_hw_output_reset_operate(DO_OPEN_RECOVERY);
					break;
			}	
		    break;
	 
	    default:             		
		    break;		
	}
    return rtl;
}

/**
  * @brief : �ֺ�բ����
  * @param : [none].
  * @return: [none] 
  * @updata: [2018-09-17][������][����]
  */
void OpeningclosingOperate(rt_uint8_t type)
{
	rt_ostimer_init(type);
}

