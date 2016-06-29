/**
  ******************************************************************************
  * @file    bsp.h
  * @author  zcd
  * @version V0.0.0
  * @date    2016年6月24日
  * @brief   该文件定义了板级LED、按键、串行口等外设资源
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

enum 
{
  false = 0,
  true = !false
};

#define pdFALSE			( ( BaseType_t ) 0 )
#define pdTRUE			( ( BaseType_t ) 1 )

#define pdPASS			( pdTRUE )
#define pdFAIL			( pdFALSE )

/* 大小端转换 */
#ifndef __SWAP

#define __swap16(A)   (( ((uint16_t)(A) & 0xff00) >> 8)    | \
                                       (( (uint16_t)(A) & 0x00ff) << 8))
                                       
#define __swap32(A)   ((( (uint32_t)(A) & 0xff000000) >> 24) | \
                                       (( (uint32_t)(A) & 0x00ff0000) >> 8)   | \
                                       (( (uint32_t)(A) & 0x0000ff00) << 8)   | \
                                       (( (uint32_t)(A) & 0x000000ff) << 24))                                       
#endif    /* #ifndef __SWAP */



/** @defgroup 导出宏定义
  * @{
  */


/**
  * @brief 串口相关
  */
#define BSP_USART_SENSOR      huart1                /* 指定与传感器通信的串口句柄 */
extern  UART_HandleTypeDef    BSP_USART_SENSOR;     /* 串口句柄1 */

#define BSP_USART_PRINT       huart2                /* 指定用来输出打印信息的串口句柄 */
extern  UART_HandleTypeDef    BSP_USART_PRINT;       /* 串口句柄2 */

#define PRINT_QUEUES_HANDLE PrintQueuesHandle
extern  osMessageQId PRINT_QUEUES_HANDLE;


 /*定义串口传输超时*/
#define TRANSMIT_OUTTIME  100                                                     
#define RECEIVE_OUTTIME   1000


/* 调试信息输出定义 */
#define SYS_DEBUG_ON              0
#define SYS_DEBUG_TRACK_ON        0

#define SYS_INFO(fmt,arg...)           printf("<<-DEBUG-INFO->>%s@%d: "fmt"\n",__func__,__LINE__,##arg)
#define SYS_ERROR(fmt,arg...)          printf("<<-DEBUG-ERROR->>%s@%d: "fmt"\n",__func__,__LINE__,##arg)
#define SYS_DEBUG(fmt,arg...)          do{\
                                          if(SYS_DEBUG_ON)\
                                          printf("<<-DEBUG->> %s@%d:"fmt"\n",__func__,__LINE__, ##arg);\
                                          }while(0)
#define SYS_DEBUG_TRACK()              do{\
                                            if(SYS_DEBUG_TRACK_ON)\
                                            printf("<<-DEBUG->> %s@%d:\n",__func__,__LINE__);\
                                         }while(0)


                                         
/**
  * @brief SPI相关
  */
#define BSP_SPI_HANDLE       hspi1              /* 指定变量名 */
extern  SPI_HandleTypeDef    BSP_SPI_HANDLE;    /* 声明 */
#define SPIx_TIMEOUT_MAX     1000              /* SPI通信最大延时 */

                                         
                                         
/**
  * @}
  */ 




/** @addtogroup BSP_Exported_Functions
  * @{
  */ 


HAL_StatusTypeDef       BSP_USART_SendData( UART_HandleTypeDef *huart, uint8_t ch );                        // BSP_USART_SendData 串口发送字符
HAL_StatusTypeDef       BSP_USART_SendArray( UART_HandleTypeDef *huart,  uint8_t* ptr, uint16_t size );     // BSP_USART_SendArray 串口发送字符串
uint8_t                 BSP_USART_ReceiveData( UART_HandleTypeDef *huart);                                  // BSP_USART_SendData 串口1接收一个字符
HAL_StatusTypeDef       BSP_USART_ReceiveArray( UART_HandleTypeDef *huart,  uint8_t* ptr, uint16_t size );  //BSP_USART_ReceiveArray 串口1发送字符
static char *itoa(int value, char *string, int radix);
void USART_printf(UART_HandleTypeDef *huart, char *Data,...);
uint8_t xSerialGetChar( osMessageQId MessageId, uint8_t *pcRxedChar, TickType_t xBlockTime );
uint8_t xSerialPutChar( osMessageQId MessageId, uint8_t cOutChar, TickType_t xBlockTime );
int fputc(int ch, FILE *f);

                                            
uint32_t SPIx_Read(void);
void SPIx_Write(uint8_t Value);
void SPIx_Error (void);

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif
  
#endif /* __BSP_H */

  
/*****************************END OF FILE****/
