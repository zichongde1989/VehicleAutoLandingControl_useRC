/**
  ******************************************************************************
  * @file    data_out.c
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-25
  * @brief   驱动DAC TLC5615输出控制值
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


/*************************** 静态函数申明 ****************************/
static void ControlOut_WriteTlc5615(uint16_t Value);
static void ControlOut_SetDACx_Voltage(uint8_t dac, float voltage);



/************************* 导出的函数定义 ****************************/
/**
  * @brief  输出初始化函数，设置默认的输出电压等
  * @param  无
  * @retval 无
  */
uint8_t ControlOut_Init( void )
{
  /* 输出俯仰控制电压 */
  //SYS_DEBUG(" DAC_PITCH=%f\n",DAC_PITCH_UOUT_CONTER);  
  ControlOut_SetDACx_Voltage( DAC_PITCH, DAC_PITCH_UOUT_CENTER );
  
  /* 输出横滚控制电压 */
  //SYS_DEBUG(" DAC_ROLL =%f\n",DAC_ROLL_UOUT_CONTER);  
  ControlOut_SetDACx_Voltage( DAC_ROLL, DAC_ROLL_UOUT_CENTER );
  
  /* 输出油门控制电压 */
  //SYS_DEBUG(" DAC_THROTTLE =%f\n",DAC_THROTTLE_UOUT_MIN);  
  ControlOut_SetDACx_Voltage( DAC_THROTTLE, DAC_THROTTLE_UOUT_MIN );
  
  return 0;
}


/**
  * @brief  输出控制值
  * @param  pControlData: 控制量
  * @retval 无
  */
uint8_t ControlOut_PutControlData( ControlData_t*  pControlData )
{
  /* 输出俯仰控制电压 */
  ControlOut_SetDACx_Voltage( DAC_PITCH, pControlData->pitch );

  /* 输出横滚控制电压 */
  ControlOut_SetDACx_Voltage( DAC_ROLL, pControlData->roll );
  
  /* 输出油门控制电压 */
  ControlOut_SetDACx_Voltage( DAC_THROTTLE, pControlData->throttle );
  
  return 0;
}


/**
  * @brief  输出中位电压，使飞行器保持位置
  * @note   在与ControlOut_PutControlData不同的线程调用此函数时，需要考虑互斥问题
  * @retval none
  */
void   ControlOut_ReleaseControl(  void  )
{
  /* 输出俯仰控制电压 */
  ControlOut_SetDACx_Voltage( DAC_PITCH, DAC_PITCH_UOUT_CENTER);

  /* 输出横滚控制电压 */
  ControlOut_SetDACx_Voltage( DAC_ROLL, DAC_ROLL_UOUT_CENTER );
  
  /* 输出油门控制电压 */
  ControlOut_SetDACx_Voltage( DAC_THROTTLE, DAC_THROTTLE_UOUT_CONTER );
}




/*************************** 静态函数定义 ****************************/
/**
  * @brief  写数据到 TLC5615 DAC0
  * @param  Value: 写入的值
  * @retval 无
  */
static void ControlOut_WriteTlc5615(uint16_t Value)
{
  uint16_t dac_value;
  uint8_t highbyte;
  uint8_t lowbyte;
  
  /* 按照DAC数据格式移动 */
  dac_value = (Value&0x3FF)<<2;
  
  /* 取出高低字节 */
  highbyte = dac_value>>8;
  lowbyte = dac_value&0xFF;
  
  /* 分别发送高、低字节 */
  SPIx_Write( highbyte );
  SPIx_Write( lowbyte );
}



/**
  * @brief  设置DAC输出电压
  * @param  DACx: 指定设置的dac
  * @param  voltage: 设定的电压值
  * @retval 无
  */
static void ControlOut_SetDACx_Voltage(uint8_t dac, float voltage)
{
  uint16_t dac_value;
  
  /* 计算电压值对应测DAC值 */
//  dac_value = 1024*voltage/(2*DAC_PITCH_Uref);
  
  
  /* 输出数据 */
  switch( dac )
  {
    /* ----pitch---- */
    case DAC_PITCH:
    {
      //SYS_DEBUG("DAC_PITCH=%f\n" ,voltage);
      
      /* ① 判断输入的电压值是否合法 */
      if( voltage < DAC_PITCH_UOUT_MIN )
      {
        voltage = DAC_PITCH_UOUT_MIN;
      }else if( voltage > DAC_PITCH_UOUT_MAX )
      {
        voltage = DAC_PITCH_UOUT_MAX ;
      }
      
      /* ② 计算电压值对应的 DAC值 */
      dac_value = 512*voltage/(DAC_PITCH_Uref);
      
      /* ③ 输出值到 DAC_PITCH  */
      DAC_PITCH_CS_LOW();
      ControlOut_WriteTlc5615( dac_value );
      DAC_PITCH_CS_HIGH();
    }
      break;
    
    
    /* ----roll---- */
    case DAC_ROLL:
    {
      //SYS_DEBUG("DAC_ROLL=%f\n" ,voltage);
      
       /* ① 判断输入的电压值是否合法 */
      if( voltage < DAC_ROLL_UOUT_MIN )
      {
        voltage = DAC_ROLL_UOUT_MIN;
      }else if( voltage > DAC_ROLL_UOUT_MAX )
      {
        voltage = DAC_ROLL_UOUT_MAX ;
      }
      
      /* ② 计算电压值对应的 DAC值 */
      dac_value = 512*voltage/(DAC_ROLL_Uref);
      
      /* ③ 输出值到 DAC_ROLL  */
      DAC_ROLL_CS_LOW();
      ControlOut_WriteTlc5615( dac_value );
      DAC_ROLL_CS_HIGH();
    }
      break;
    
    
    /* ----throttle---- */
    case DAC_THROTTLE:
    {
      //SYS_DEBUG("DAC_THROTTLE=%f\n" ,voltage);
      
       /* ① 判断输入的电压值是否合法 */
      if( voltage < DAC_THROTTLE_UOUT_MIN )
      {
        voltage = DAC_THROTTLE_UOUT_MIN;
      }else if( voltage > DAC_THROTTLE_UOUT_MAX )
      {
        voltage = DAC_THROTTLE_UOUT_MAX ;
      }
      
       /* ② 计算电压值对应的 DAC值 */
      dac_value = 512*voltage/(DAC_THROTTLE_Uref);
      
      /* ③ 输出值到 DAC_THROTTLE  */
      DAC_THROTTLE_CS_LOW();
      ControlOut_WriteTlc5615( dac_value );
      DAC_THROTTLE_CS_HIGH();
    }
      break;
      
    default:
      break;
  }
}

