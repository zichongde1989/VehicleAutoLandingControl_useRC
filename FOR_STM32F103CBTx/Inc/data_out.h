/**
  ******************************************************************************
  * @file    data_out.h
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-25
  * @brief   ����������� ͷ�ļ�
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
  * @brief  ����DAC
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
  * @brief  ����DAC TLC5615��Ƭѡ�źŲ�����
  */
#define DAC_PITCH_CS_LOW()        HAL_GPIO_WritePin(DAC_CS0_GPIO_Port, DAC_CS0_Pin, GPIO_PIN_RESET)
#define DAC_PITCH_CS_HIGH()       HAL_GPIO_WritePin(DAC_CS0_GPIO_Port, DAC_CS0_Pin, GPIO_PIN_SET)

#define DAC_ROLL_CS_LOW()         HAL_GPIO_WritePin(DAC_CS1_GPIO_Port, DAC_CS1_Pin, GPIO_PIN_RESET)
#define DAC_ROLL_CS_HIGH()        HAL_GPIO_WritePin(DAC_CS1_GPIO_Port, DAC_CS1_Pin, GPIO_PIN_SET)

#define DAC_THROTTLE_CS_LOW()     HAL_GPIO_WritePin(DAC_CS2_GPIO_Port, DAC_CS2_Pin, GPIO_PIN_RESET)
#define DAC_THROTTLE_CS_HIGH()    HAL_GPIO_WritePin(DAC_CS2_GPIO_Port, DAC_CS2_Pin, GPIO_PIN_SET)

/**
  * @brief  ����DAC�Ĳο���ѹ��ƫ��
  */
#define DAC_PITCH_Uref      1.8
#define DAC_ROLL_Uref       1.8  
#define DAC_THROTTLE_Uref   1.8  

#define DAC_PITCH_OFFSET      0  
#define DAC_ROLL_OFFSET       0 
#define DAC_THROTTLE_OFFSET   0  

/* �����ѹ���� */
#define DAC_PITCH_UOUT_MIN        0.42   /* ��������ѹ����λV */
#define DAC_ROLL_UOUT_MIN         0.48 
#define DAC_THROTTLE_UOUT_MIN     0.38

#define DAC_PITCH_UOUT_MAX        2.98    /* ��������ѹ����λV */ 
#define DAC_ROLL_UOUT_MAX         3.02 
#define DAC_THROTTLE_UOUT_MAX     2.92  

/* �����ѹ�������� */
#define DAC_PITCH_UOUT_CENTER     1.74   /* �����ѹ�м�ֵ����λV */  
#define DAC_ROLL_UOUT_CENTER      1.76
#define DAC_THROTTLE_UOUT_CONTER  1.60  

/* �����ѹ�������*/
#define DAC_PITCH_UOUT_DIRECTIOPN     (1)   /* �����ԣ�pitch������ʱ����ѹ����    */  
#define DAC_ROLL_UOUT_DIRECTIOPN      (-1)   /* �����ԣ�roll������ʱ��ѹ��С       */
#define DAC_THROTTLE_UOUT_DIRECTIOPN  (1)    /* �����ԣ�throttle����ǰʱ����ѹ��� */


/* �����ѹ������-�޶����п��ƹ��̵ĵ��ڷ�Χ */
#define DAC_UOUT_CONTROL_ZONE_SCAL       1.0    /* ���������ţ�����ʱ������������ɵ��ڷ�Χ����֤���а�ȫ �� */  
//TODO::������Ϻ󣬽���ֵֵΪ1

#define DAC_PITCH_UOUT_CONTROL_ZONE       (  ( (DAC_PITCH_UOUT_MAX   ) - (DAC_PITCH_UOUT_CENTER   ) )/DAC_UOUT_CONTROL_ZONE_SCAL  )   
#define DAC_ROLL_UOUT_CONTROL_ZONE        (  ( (DAC_ROLL_UOUT_MAX    ) - (DAC_ROLL_UOUT_CENTER    ) )/DAC_UOUT_CONTROL_ZONE_SCAL  )   
#define DAC_THROTTLE_UOUT_CONTROL_ZONE    (  ( (DAC_THROTTLE_UOUT_MAX) - (DAC_THROTTLE_UOUT_CONTER) )/DAC_UOUT_CONTROL_ZONE_SCAL  )     


/* Ĭ�ϵ������ѹ */
#define DAC_PITCH_DERFALT_OUT     (DAC_PITCH_UOUT_CENTER)   /* �����ѹ�м�ֵ����λV */  
#define DAC_ROLL_DERFALT_OUT      (DAC_ROLL_UOUT_CENTER)
#define DAC_THROTTLE_DERFALT_OUT  (DAC_THROTTLE_UOUT_MIN)  






/************************* �����ĺ������� ****************************/
uint8_t ControlOut_Init( void );
uint8_t ControlOut_PutControlData( ControlData_t*  pControlData );

#endif

