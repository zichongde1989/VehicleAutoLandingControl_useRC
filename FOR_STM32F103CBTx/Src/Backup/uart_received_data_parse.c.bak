/**
  ******************************************************************************
  * @file    uart_received_data_parse.c
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-23
  * @brief   超声波定位数据包接收以及解析
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
#include <string.h>
#include "data_typedef.h"
#include "cmsis_os.h"
#include "uart_received_data_parse.h"
#include "bsp.h"


/**
  * @brief  从串口接收队列src_queue中取出数据进行解析，解析完毕后发送至处理目标队列dst_queue中
  * @param  src_queue:  串口接收队列
  * @param  dst_queue:  超声波数据缓冲队列（仅发地址）
  * 
  * @retval 无（死循环）
  */
void ReceivedDataParseLoop(osMessageQId src_queue, osMessageQId dst_queue )
{
  ;
}


/**
  * @brief  将数据 SensorData_t 发送出去
  * @param  SensorData:  待发送的数据内容
  * 
  * @retval 无（死循环）
  */
void DataParse_PutData(SensorData_t* SensorData)
{
  Protocol_SensorPack_t Protocol_SensorPack;
  
  /* 填写帧文件头 */
  Protocol_SensorPack.header0 = SENSOR_PROTOCOL_PACK_HEADER0;
  Protocol_SensorPack.header1 = SENSOR_PROTOCOL_PACK_HEADER1;
  
  /* 填写帧数据区长度字段*/
  Protocol_SensorPack.length      = SENSOR_PROTOCOL_PACK_MAX_DATA_LEN ;
  
  /* 将要发送的数据复制到数据区 */
  memcpy( &Protocol_SensorPack.sensor_data, SensorData, Protocol_SensorPack.length );
  
  /* 计算并填写校验字 */
  Protocol_SensorPack.sum = doSumCheck( (uint8_t*)&Protocol_SensorPack.sensor_data, Protocol_SensorPack.length );
  
  
  /* 数据发送 */
  BSP_USART_SendArray( &BSP_USART_SENSOR,  (uint8_t*)&Protocol_SensorPack, SENSOR_PROTOCOL_PACK_MAX_SIZE );
  
}




/**
  * @brief  对数据进行校验，获取校验值
  * @param  ptr: 待校验数据指针
  * @param  len: 待校验数据长度
  * 
  * @retval uint8_t: 校验值
  */
uint8_t doSumCheck(uint8_t* ptr, uint8_t len )
{
  uint8_t sum;
  uint8_t i;
  
  sum = 0;
  for ( i=0;i<len;i++ )
  {
    sum += *ptr++;
  }
  
  return sum;
}


#if 0
/**
  * @brief  将一个2位十进制数转换为2进制
  * @param  Value: 待转换的值
  * @retval 转换后的值
  */
static uint8_t ByteToBcd2(uint8_t Value)
{
  uint32_t bcdhigh = 0;
  
  while(Value >= 10)
  {
    bcdhigh++;
    Value -= 10;
  }
  
  return  ((uint8_t)(bcdhigh << 4) | Value);
}

/**
  * @brief  将2位BCD数转换为十进制
  * @param  Value: 待转换的BCD数
  * @retval 转换结果
  */
static uint8_t Bcd2ToByte(uint8_t Value)
{
  uint32_t tmp = 0;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
}

#endif
