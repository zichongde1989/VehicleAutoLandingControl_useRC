/**
  ******************************************************************************
  * @file    data_out.h
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-25
  * @brief   数据输出控制 头文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
#ifndef  __DATA_OUT_H__
#define  __DATA_OUT_H__

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

#include "data_typedef.h"



/**
  * @brief  定义DAC
  */
typedef enum 
{
  DAC0 = 0, 
  DAC1,
  DAC2,
  DAC_PITCH     = DAC0,
  DAC_ROLL      = DAC1,
  DAC_THROTTLE  = DAC2,
} DACx_t;


/**
  * @brief  定义DAC TLC5615的片选信号操作宏
  */
#define DAC_PITCH_CS_LOW()        HAL_GPIO_WritePin(DAC_CS0_GPIO_Port, DAC_CS0_Pin, GPIO_PIN_RESET)
#define DAC_PITCH_CS_HIGH()       HAL_GPIO_WritePin(DAC_CS0_GPIO_Port, DAC_CS0_Pin, GPIO_PIN_SET)

#define DAC_ROLL_CS_LOW()         HAL_GPIO_WritePin(DAC_CS1_GPIO_Port, DAC_CS1_Pin, GPIO_PIN_RESET)
#define DAC_ROLL_CS_HIGH()        HAL_GPIO_WritePin(DAC_CS1_GPIO_Port, DAC_CS1_Pin, GPIO_PIN_SET)

#define DAC_THROTTLE_CS_LOW()     HAL_GPIO_WritePin(DAC_CS2_GPIO_Port, DAC_CS2_Pin, GPIO_PIN_RESET)
#define DAC_THROTTLE_CS_HIGH()    HAL_GPIO_WritePin(DAC_CS2_GPIO_Port, DAC_CS2_Pin, GPIO_PIN_SET)

/**
  * @brief  定义DAC的参考电压和偏移
  */
#define DAC_PITCH_Uref      1.8
#define DAC_ROLL_Uref       1.8  
#define DAC_THROTTLE_Uref   1.8  

#define DAC_PITCH_OFFSET      0  
#define DAC_ROLL_OFFSET       0 
#define DAC_THROTTLE_OFFSET   0  

/* 输出电压限制 */
#define DAC_PITCH_UOUT_MIN        0.5   /* 最低输出电压，单位V */
#define DAC_ROLL_UOUT_MIN         0.5 
#define DAC_THROTTLE_UOUT_MIN     0.5

#define DAC_PITCH_UOUT_MAX        3.1    /* 最高输出电压，单位V */ 
#define DAC_ROLL_UOUT_MAX         3.1 
#define DAC_THROTTLE_UOUT_MAX     3.1  

/* 输出电压控制中心 */
#define DAC_PITCH_UOUT_CONTER     1.75   /* 输出电压中间值，单位V */  
#define DAC_ROLL_UOUT_CONTER      1.75
#define DAC_THROTTLE_UOUT_CONTER  1.75  


/* 默认的输出电压 */
#define DAC_PITCH_DERFALT_OUT     (DAC_PITCH_UOUT_CONTER)   /* 输出电压中间值，单位V */  
#define DAC_ROLL_DERFALT_OUT      (DAC_ROLL_UOUT_CONTER)
#define DAC_THROTTLE_DERFALT_OUT  (DAC_THROTTLE_UOUT_MIN)  






/************************* 导出的函数定义 ****************************/
uint8_t ControlOut_Init( void );
uint8_t ControlOut_PutControlData( ControlData_t*  pControlData );

#endif

