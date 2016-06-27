/**
  ******************************************************************************
  * @file    uart_received_data_parse.c
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-23
  * @brief   ��������λ���ݰ������Լ�����
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
  * @brief  �Ӵ��ڽ��ն���src_queue��ȡ�����ݽ��н�����������Ϻ���������Ŀ�����dst_queue��
  * @param  src_queue:  ���ڽ��ն���
  * @param  dst_queue:  ���������ݻ�����У�������ַ��
  * 
  * @retval �ޣ���ѭ����
  */
void ReceivedDataParseLoop(osMessageQId src_queue, osMessageQId dst_queue )
{
  ;
}


/**
  * @brief  ������ SensorData_t ���ͳ�ȥ
  * @param  SensorData:  �����͵���������
  * 
  * @retval �ޣ���ѭ����
  */
void DataParse_PutData(SensorData_t* SensorData)
{
  Protocol_SensorPack_t Protocol_SensorPack;
  
  /* ��д֡�ļ�ͷ */
  Protocol_SensorPack.header0 = SENSOR_PROTOCOL_PACK_HEADER0;
  Protocol_SensorPack.header1 = SENSOR_PROTOCOL_PACK_HEADER1;
  
  /* ��д֡�����������ֶ�*/
  Protocol_SensorPack.length      = SENSOR_PROTOCOL_PACK_MAX_DATA_LEN ;
  
  /* ��Ҫ���͵����ݸ��Ƶ������� */
  memcpy( &Protocol_SensorPack.sensor_data, SensorData, Protocol_SensorPack.length );
  
  /* ���㲢��дУ���� */
  Protocol_SensorPack.sum = doSumCheck( (uint8_t*)&Protocol_SensorPack.sensor_data, Protocol_SensorPack.length );
  
  
  /* ���ݷ��� */
  BSP_USART_SendArray( &BSP_USART_SENSOR,  (uint8_t*)&Protocol_SensorPack, SENSOR_PROTOCOL_PACK_MAX_SIZE );
  
}




/**
  * @brief  �����ݽ���У�飬��ȡУ��ֵ
  * @param  ptr: ��У������ָ��
  * @param  len: ��У�����ݳ���
  * 
  * @retval uint8_t: У��ֵ
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
  * @brief  ��һ��2λʮ������ת��Ϊ2����
  * @param  Value: ��ת����ֵ
  * @retval ת�����ֵ
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
  * @brief  ��2λBCD��ת��Ϊʮ����
  * @param  Value: ��ת����BCD��
  * @retval ת�����
  */
static uint8_t Bcd2ToByte(uint8_t Value)
{
  uint32_t tmp = 0;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
}

#endif
