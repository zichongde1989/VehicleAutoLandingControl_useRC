/**
  ******************************************************************************
  * @file    data_process.h
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-25
  * @brief   ���ݴ������ͷ�ļ�
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 

#ifndef __DATA_PROCESS_H__
#define __DATA_PROCESS_H__

#include "data_typedef.h"
#include "cmsis_os.h"





/**
  * @brief  �Դ��������ݽ��д��������������
  * @param  ptr: ��У������ָ��
  * @param  len: ��У�����ݳ���
  * 
  * @retval uint8_t: У��ֵ
  */
uint8_t   DataProcess_DoProcess( SensorData_t* pSensorData, ControlData_t* pControlData  );




#endif
