/**
  ******************************************************************************
  * @file    data_typedef.h
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-23
  * @brief   ������ģ����������ݸ�ʽ��ض���
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
  
  
#ifndef  __DATA_TYPEDEF__
#define  __DATA_TYPEDEF__

#include "stm32f1xx_hal.h"


/* ע�⣺���½ṹ����Ҫ����1�ֽڶ��� */
#pragma pack(push,1)  //������뷽ʽ


/** 
  * @brief  ��������λװ����������ݽṹ
  */
typedef struct SensorDataStruct
{
  uint32_t  time_boot_ms;         /*!< ��λmS,�Ӵ�����ϵͳ������ʼ��ʱ       */
  int16_t   x;                    /*!< �ֲ�����ϵ����λ:mm                   */
  int16_t   y;                    /*!< �ֲ�����ϵ����λ:mm                   */
  int16_t   z;                    /*!< �ֲ�����ϵ����λ:mm                   */
  float     convariance;          /*!< ������������Ŷȣ����������ģ�����  */
}SensorData_t;
/* ���û��������뷽ʽ */
#pragma pack(pop)     



/** @defgroup ��������λװ��ͨ�����ڷ��͵�����֡��غ궨��
  * @{
  */
#define SENSOR_PROTOCOL_PACK_MAX_DATA_LEN   sizeof(SensorData_t)
#define SENSOR_PROTOCOL_PACK_MAX_SIZE       (SENSOR_PROTOCOL_PACK_MAX_DATA_LEN + 2 + 1 + 1)
#define SENSOR_PROTOCOL_PACK_HEADER0        0x7E
#define SENSOR_PROTOCOL_PACK_HEADER1        0x55

/**
  * @}
  */


/* ע�⣺���½ṹ����Ҫ����1�ֽڶ��� */
#pragma pack(push,1)  //������뷽ʽ

/** 
  * @brief  ��������λװ��ͨ�����ڷ��͵�Э��֡��ʽ
  */
typedef struct Protocol_SensorPackStruct
{
  uint8_t         header0;          /*!< ֡ͷ0                                */
  uint8_t         header1;          /*!< ֡ͷ1                                */
  uint8_t         length;           /*!< ֡�����ݶγ��ȣ����99               */
  SensorData_t    sensor_data;      /*!< ������������ݣ����͹�����BCD���װ  */
  uint8_t         sum;              /*!< ����У��ֵ�������ۼӺ�У��           */
}Protocol_SensorPack_t;



/* ���û��������뷽ʽ */
#pragma pack(pop)     





/** 
  * @brief �����������
  */
typedef struct ControlDataStruct
{
  float   pitch;           /*!< �����ǿ���ֵ����Χ��0-5������ֵ��2.5��Ĭ��ֵ��2.5      */
  float   roll;            /*!< ����ǿ���ֵ����Χ��0-5������ֵ��2.5��Ĭ��ֵ��2.5      */
  float   throttle;        /*!< ���ſ���ֵ��  ��Χ��0-5������ֵ��2.5��Ĭ��ֵ��0        */
}ControlData_t;



#endif
