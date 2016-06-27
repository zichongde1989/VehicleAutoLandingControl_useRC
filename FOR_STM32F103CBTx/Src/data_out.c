/**
  ******************************************************************************
  * @file    data_out.c
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-25
  * @brief   ����DAC TLC5615�������ֵ
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "data_typedef.h"
#include "bsp.h"
#include "data_out.h"


/*************************** ��̬�������� ****************************/
static void ControlOut_WriteTlc5615(uint16_t Value);
static void ControlOut_SetDACx_Voltage(uint8_t dac, float voltage);



/************************* �����ĺ������� ****************************/
/**
  * @brief  �����ʼ������������Ĭ�ϵ������ѹ��
  * @param  ��
  * @retval ��
  */
uint8_t ControlOut_Init( void )
{
  /* ����������Ƶ�ѹ */
  //SYS_DEBUG(" DAC_PITCH=%f\n",DAC_PITCH_UOUT_CONTER);  
  ControlOut_SetDACx_Voltage( DAC_PITCH, DAC_PITCH_UOUT_CONTER );
  
  /* ���������Ƶ�ѹ */
  //SYS_DEBUG(" DAC_ROLL =%f\n",DAC_ROLL_UOUT_CONTER);  
  ControlOut_SetDACx_Voltage( DAC_ROLL, DAC_ROLL_UOUT_CONTER );
  
  /* ������ſ��Ƶ�ѹ */
  //SYS_DEBUG(" DAC_THROTTLE =%f\n",DAC_THROTTLE_UOUT_MIN);  
  ControlOut_SetDACx_Voltage( DAC_THROTTLE, DAC_THROTTLE_UOUT_MIN );
  
  return 0;
}


/**
  * @brief  �������ֵ
  * @param  pControlData: ������
  * @retval ��
  */
uint8_t ControlOut_PutControlData( ControlData_t*  pControlData )
{
  /* ����������Ƶ�ѹ */
  ControlOut_SetDACx_Voltage( DAC_PITCH, pControlData->pitch );

  /* ���������Ƶ�ѹ */
  ControlOut_SetDACx_Voltage( DAC_ROLL, pControlData->roll );
  
  /* ������ſ��Ƶ�ѹ */
  ControlOut_SetDACx_Voltage( DAC_THROTTLE, pControlData->throttle );
  
  return 0;
}






/*************************** ��̬�������� ****************************/
/**
  * @brief  д���ݵ� TLC5615 DAC0
  * @param  Value: д���ֵ
  * @retval ��
  */
static void ControlOut_WriteTlc5615(uint16_t Value)
{
  uint16_t dac_value;
  uint8_t highbyte;
  uint8_t lowbyte;
  
  /* ����DAC���ݸ�ʽ�ƶ� */
  dac_value = (Value&0x3FF)<<2;
  
  /* ȡ���ߵ��ֽ� */
  highbyte = dac_value>>8;
  lowbyte = dac_value&0xFF;
  
  /* �ֱ��͸ߡ����ֽ� */
  SPIx_Write( highbyte );
  SPIx_Write( lowbyte );
}



/**
  * @brief  ����DAC�����ѹ
  * @param  DACx: ָ�����õ�dac
  * @param  voltage: �趨�ĵ�ѹֵ
  * @retval ��
  */
static void ControlOut_SetDACx_Voltage(uint8_t dac, float voltage)
{
  uint16_t dac_value;
  
  /* �����ѹֵ��Ӧ��DACֵ */
//  dac_value = 1024*voltage/(2*DAC_PITCH_Uref);
  
  
  /* ������� */
  switch( dac )
  {
    /* ----pitch---- */
    case DAC_PITCH:
    {
      //SYS_DEBUG("DAC_PITCH=%f\n" ,voltage);
      
      /* �� �ж�����ĵ�ѹֵ�Ƿ�Ϸ� */
      if( voltage < DAC_PITCH_UOUT_MIN )
      {
        voltage = DAC_PITCH_UOUT_MIN;
      }else if( voltage > DAC_PITCH_UOUT_MAX )
      {
        voltage = DAC_PITCH_UOUT_MAX ;
      }
      
      /* �� �����ѹֵ��Ӧ�� DACֵ */
      dac_value = 512*voltage/(DAC_PITCH_Uref);
      
      /* �� ���ֵ�� DAC_PITCH  */
      DAC_PITCH_CS_LOW();
      ControlOut_WriteTlc5615( dac_value );
      DAC_PITCH_CS_HIGH();
    }
      break;
    
    
    /* ----roll---- */
    case DAC_ROLL:
    {
      //SYS_DEBUG("DAC_ROLL=%f\n" ,voltage);
      
       /* �� �ж�����ĵ�ѹֵ�Ƿ�Ϸ� */
      if( voltage < DAC_ROLL_UOUT_MIN )
      {
        voltage = DAC_ROLL_UOUT_MIN;
      }else if( voltage > DAC_ROLL_UOUT_MAX )
      {
        voltage = DAC_ROLL_UOUT_MAX ;
      }
      
      /* �� �����ѹֵ��Ӧ�� DACֵ */
      dac_value = 512*voltage/(DAC_ROLL_Uref);
      
      /* �� ���ֵ�� DAC_ROLL  */
      DAC_ROLL_CS_LOW();
      ControlOut_WriteTlc5615( dac_value );
      DAC_ROLL_CS_HIGH();
    }
      break;
    
    
    /* ----throttle---- */
    case DAC_THROTTLE:
    {
      //SYS_DEBUG("DAC_THROTTLE=%f\n" ,voltage);
      
       /* �� �ж�����ĵ�ѹֵ�Ƿ�Ϸ� */
      if( voltage < DAC_THROTTLE_UOUT_MIN )
      {
        voltage = DAC_THROTTLE_UOUT_MIN;
      }else if( voltage > DAC_THROTTLE_UOUT_MAX )
      {
        voltage = DAC_THROTTLE_UOUT_MAX ;
      }
      
       /* �� �����ѹֵ��Ӧ�� DACֵ */
      dac_value = 512*voltage/(DAC_THROTTLE_Uref);
      
      /* �� ���ֵ�� DAC_THROTTLE  */
      DAC_THROTTLE_CS_LOW();
      ControlOut_WriteTlc5615( dac_value );
      DAC_THROTTLE_CS_HIGH();
    }
      break;
      
    default:
      break;
  }
}

