/**
  ******************************************************************************
  * @file    data_typedef.h
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-23
  * @brief   超声波模块输出的数据格式相关定义
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
  
  
#ifndef  __DATA_TYPEDEF__
#define  __DATA_TYPEDEF__

#include "stm32f1xx_hal.h"


/* 注意：以下结构体需要设置1字节对齐 */
#pragma pack(push,1)  //保存对齐方式


/** 
  * @brief  超声波定位装置输出的数据结构
  */
typedef struct SensorDataStruct
{
  uint32_t  time_boot_ms;         /*!< 单位mS,从传感器系统启动开始计时       */
  int16_t   x;                    /*!< 局部坐标系，单位:mm                   */
  int16_t   y;                    /*!< 局部坐标系，单位:mm                   */
  int16_t   z;                    /*!< 局部坐标系，单位:mm                   */
  float     convariance;          /*!< 传感器输出可信度，由冗余计算模型算出  */
}SensorData_t;
/* 设置回其他对齐方式 */
#pragma pack(pop)     



/** @defgroup 超声波定位装置通过串口发送的数据帧相关宏定义
  * @{
  */
#define SENSOR_PROTOCOL_PACK_MAX_DATA_LEN   sizeof(SensorData_t)
#define SENSOR_PROTOCOL_PACK_MAX_SIZE       (SENSOR_PROTOCOL_PACK_MAX_DATA_LEN + 2 + 1 + 1)
#define SENSOR_PROTOCOL_PACK_HEADER0        0x7E
#define SENSOR_PROTOCOL_PACK_HEADER1        0x55

/**
  * @}
  */


/* 注意：以下结构体需要设置1字节对齐 */
#pragma pack(push,1)  //保存对齐方式

/** 
  * @brief  超声波定位装置通过串口发送的协议帧格式
  */
typedef struct Protocol_SensorPackStruct
{
  uint8_t         header0;          /*!< 帧头0                                */
  uint8_t         header1;          /*!< 帧头1                                */
  uint8_t         length;           /*!< 帧内数据段长度，最大99               */
  SensorData_t    sensor_data;      /*!< 传感器输出数据，发送过程以BCD码封装  */
  uint8_t         sum;              /*!< 数据校验值，采用累加和校验           */
}Protocol_SensorPack_t;



/* 设置回其他对齐方式 */
#pragma pack(pop)     





/** 
  * @brief 控制数据输出
  */
typedef struct ControlDataStruct
{
  float   pitch;           /*!< 俯仰角控制值，范围：0-5，中心值：2.5，默认值：2.5      */
  float   roll;            /*!< 横滚角控制值，范围：0-5，中心值：2.5，默认值：2.5      */
  float   throttle;        /*!< 油门控制值，  范围：0-5，中心值：2.5，默认值：0        */
}ControlData_t;



#endif
