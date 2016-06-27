/**
  ******************************************************************************
  * @file    data_process.c
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-24
  * @brief   主控制函数
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 

#include "data_typedef.h"
#include "cmsis_os.h"
#include "data_process.h"
#include "data_out.h"





/**
  * @brief  对传感器数据进行处理，输出控制数据
  * @param  ptr: 待校验数据指针
  * @param  len: 待校验数据长度
  * 
  * @retval uint8_t: 校验值
  */
uint8_t   DataProcess_DoProcess( SensorData_t* pSensorData, ControlData_t* pControlData  )
{
  /* 计算 */
//  printf("\nReceived sensor data:\n");
//  printf("time_boot_ms:%d\n",pSensorData->time_boot_ms);
//  printf("x:%d\n",pSensorData->x);
//  printf("y:%d\n",pSensorData->y);
//  printf("z:%d\n",pSensorData->z);
//  printf("convariance:%f\n",pSensorData->convariance);
  
  pControlData->pitch = 2.5;
  pControlData->roll = 2.5;
  pControlData->throttle = 0.0;

  
  return 0;
}
