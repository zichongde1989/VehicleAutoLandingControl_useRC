/**
  ******************************************************************************
  * @file    uart_received_data_parse.h
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-23
  * @brief   超声波定位数据解析头文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 

#ifndef __UART_RECEIVED_DATA_PARSE__
#define __UART_RECEIVED_DATA_PARSE__
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "data_typedef.h"


void ReceivedDataParseLoop(osMessageQId src_queue, osMessageQId dst_queue );
uint8_t doSumCheck(uint8_t* ptr, uint8_t len );
void DataParse_PutData(SensorData_t* SensorData);

#if 0
static uint8_t ByteToBcd2(uint8_t Value);
static uint8_t Bcd2ToByte(uint8_t Value);

#endif


#endif

