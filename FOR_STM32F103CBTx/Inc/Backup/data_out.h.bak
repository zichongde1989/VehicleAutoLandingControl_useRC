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
#define DAC_PITCH_UOUT_MIN        0.42   /* 最低输出电压，单位V */
#define DAC_ROLL_UOUT_MIN         0.48 
#define DAC_THROTTLE_UOUT_MIN     0.38

#define DAC_PITCH_UOUT_MAX        2.98    /* 最高输出电压，单位V */ 
#define DAC_ROLL_UOUT_MAX         3.02 
#define DAC_THROTTLE_UOUT_MAX     2.92  

/* 输出电压控制中心 */
#define DAC_PITCH_UOUT_CENTER     1.74   /* 输出电压中间值，单位V */  
#define DAC_ROLL_UOUT_CENTER      1.76
#define DAC_THROTTLE_UOUT_CONTER  1.60  

/* 输出电压方向控制*/
#define DAC_PITCH_UOUT_DIRECTIOPN     (-1)   /* 经测试，pitch杆向下时，电压增大    */  
#define DAC_ROLL_UOUT_DIRECTIOPN      (-1)   /* 经测试，roll杆向右时电压变小       */
#define DAC_THROTTLE_UOUT_DIRECTIOPN  (1)    /* 经测试，throttle杆向前时，电压变大 */


/* 输出电压调节区-限定飞行控制过程的调节范围 */
#define DAC_UOUT_CONTROL_ZONE_SCAL       1.0    /* 调节区缩放，调试时用来控制输出可调节范围，保证飞行安全 。 */  
//TODO::调试完毕后，将该值值为1

#define DAC_PITCH_UOUT_CONTROL_ZONE       (  ( (DAC_PITCH_UOUT_MAX   ) - (DAC_PITCH_UOUT_CENTER   ) )/DAC_UOUT_CONTROL_ZONE_SCAL  )   
#define DAC_ROLL_UOUT_CONTROL_ZONE        (  ( (DAC_ROLL_UOUT_MAX    ) - (DAC_ROLL_UOUT_CENTER    ) )/DAC_UOUT_CONTROL_ZONE_SCAL  )   
#define DAC_THROTTLE_UOUT_CONTROL_ZONE    (  ( (DAC_THROTTLE_UOUT_MAX) - (DAC_THROTTLE_UOUT_CONTER) )/DAC_UOUT_CONTROL_ZONE_SCAL  )     


/* 默认的输出电压 */
#define DAC_PITCH_DERFALT_OUT     (DAC_PITCH_UOUT_CENTER)   /* 输出电压中间值，单位V */  
#define DAC_ROLL_DERFALT_OUT      (DAC_ROLL_UOUT_CENTER)
#define DAC_THROTTLE_DERFALT_OUT  (DAC_THROTTLE_UOUT_MIN)  






/************************* 导出的函数定义 ****************************/
uint8_t ControlOut_Init( void );
uint8_t ControlOut_PutControlData( ControlData_t*  pControlData );
void   ControlOut_ReleaseControl(  void  );

#endif

