/**
  ******************************************************************************
  * @file    bsp.c
  * @author  字崇德
  * @version V1.0.0
  * @date    2015-11-12
  * @brief   板级资源定义
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"


/** @defgroup  全局变量
  * @{
  */ 
uint32_t SpixTimeout = SPIx_TIMEOUT_MAX;           /*<! Value of Timeout when SPI communication fails */

/**
  * @}
  */ 




/** @defgroup  函数定义
  * @{
  */ 

/*************************************************************
* @brief BSP_USART_SendData 串口发送字符
* @param ch:待发送字符
* @arg
* @retval 发送成功标志位
**************************************************************/
HAL_StatusTypeDef BSP_USART_SendData(UART_HandleTypeDef *huart, uint8_t ch)
{
 
  /*等待上次发送完成*/
  while( (HAL_UART_STATE_BUSY_TX == HAL_UART_GetState(huart) )||(HAL_UART_STATE_BUSY_TX_RX == HAL_UART_GetState(huart)) );  

  /*开始发送数据-阻塞方式*/
  if( HAL_UART_Transmit(huart, &ch, 1,TRANSMIT_OUTTIME) == HAL_UART_STATE_TIMEOUT )
    return HAL_TIMEOUT; /*发送超时*/

  return HAL_OK;
}


/*************************************************************
* @brief BSP_USART_SendArray 串口发送字符串
* @param ptr:待发送数组，size：发送的字节数
* @arg
* @retval 发送状态标识
**************************************************************/
HAL_StatusTypeDef BSP_USART_SendArray(UART_HandleTypeDef *huart,  uint8_t* ptr,uint16_t size)
{
  if(NULL == ptr)
    return HAL_ERROR;
  

  if(size)
  {
    /*等待上次发送完成*/
    while( (HAL_UART_STATE_BUSY_TX == HAL_UART_GetState(huart) )||(HAL_UART_STATE_BUSY_TX_RX == HAL_UART_GetState(huart)) );  

    
    /*开始发送数据-阻塞方式*/
    if( HAL_UART_Transmit(huart, ptr, size,TRANSMIT_OUTTIME) == HAL_UART_STATE_TIMEOUT )
      return HAL_TIMEOUT; /*发送超时*/
  
  }

  return HAL_OK;  
}



/*************************************************************
* @brief BSP_USART_SendData 串口接收一个字符
* @param 
* @arg
* @retval ch: 返回接收到的字符
**************************************************************/
uint8_t BSP_USART_ReceiveData(UART_HandleTypeDef *huart )
{
  uint8_t ch;
 
  /*等待上次接收完成*/
  while( (HAL_UART_STATE_BUSY_RX == HAL_UART_GetState(huart) )||(HAL_UART_STATE_BUSY_TX_RX  == HAL_UART_GetState(huart)) );  

  /*开始接收数据*/
  if( HAL_UART_STATE_TIMEOUT  ==  HAL_UART_Receive(huart, &ch, 1,RECEIVE_OUTTIME) )
    return 0;//HAL_TIMEOUT; /*接收超时，返回0*/
 
  return ch;
}



/*************************************************************
* @brief BSP_USART_ReceiveArray 串口发送字符
* @param huart：句柄，ptr:待存放数组，size：发送的字节数
* @arg
* @retval 接收状态标识
**************************************************************/
HAL_StatusTypeDef BSP_USART_ReceiveArray(UART_HandleTypeDef *huart,  uint8_t* ptr,uint16_t size)
{
  if(NULL == ptr)
    return HAL_ERROR;
  
  if(size)
  {
    /*等待上次接收完成*/
    while( (HAL_UART_STATE_BUSY_RX == HAL_UART_GetState(huart) )||(HAL_UART_STATE_BUSY_TX_RX  == HAL_UART_GetState(huart)) );  

    /*开始接收数据*/
    if(HAL_UART_STATE_TIMEOUT  ==  HAL_UART_Receive(huart, ptr, size ,RECEIVE_OUTTIME) )
      return HAL_TIMEOUT;//; /*接收超时，返回0*/
    
  }

  return HAL_OK;  
}



/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	
	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
	    *ptr = 0;
	    return string;
	}
	
	if (!value)
	{
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;
	}
	
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
	    *ptr++ = '-';
	
	    /* Make the value positive. */
	    value *= -1;
	}
	
	for (i = 10000; i > 0; i /= 10)
	{
	    d = value / i;
	
	    if (d || flag)
	    {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}
	
	/* Null terminate the string. */
	*ptr = 0;
	
	return string;

} /* NCL_Itoa */



/*
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART_printf( USART1, "\r\n %s \r\n", j );
 */
void USART_printf(UART_HandleTypeDef *huart, char *Data,...)
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
        if ( *Data == 0x5c )  //'\'
        {									  
            switch ( *++Data )
            {
                case 'r':							          //回车符
                    BSP_USART_SendData(huart,0x0d);
                    Data ++;
                break;

                case 'n':							          //换行符
                    BSP_USART_SendData(huart,0x0a);	
                    Data ++;
                break;

                default:
                    Data ++;
                break;
            }			 
        }
        else if ( *Data == '%')
        {									  //
            switch ( *++Data )
            {				
                case 's':										  //字符串
                    s = va_arg(ap, const char *);
                    for ( ; *s; s++) 
                    {
                       BSP_USART_SendData(huart,*s);
                    }
                    Data++;
                    break;

                case 'd':										//十进制
                    d = va_arg(ap, int);
                    itoa(d, buf, 10);
                    for (s = buf; *s; s++) 
                    {
                        BSP_USART_SendData(huart,*s);
                    }
                    Data++;
                    break;
                default:
                    Data++;
                    break;
            }		 
      } /* end of else if */
      else BSP_USART_SendData(huart,*Data++);
	}

}

/************************************串口队列操作函数***************************************/

/**
  * @brief  从串口接收队列中获取数据
  * @param  MessageId: 队列名称
  * @param  pcRxedChar: 指向目标储存位置
  * @param  xBlockTime: 等待时间
  * @retval uint8_t: 成功失败标识 pdPASS/pdFAIL
  */
uint8_t xSerialGetChar( osMessageQId MessageId, uint8_t *pcRxedChar, TickType_t xBlockTime )
{
  osEvent  evt;
  
  /* 查收显示数据 */
  evt = osMessageGet( MessageId, xBlockTime );  
  if (evt.status == osEventMessage) 
  {
    *pcRxedChar = (uint8_t)evt.value.v;
    return pdPASS;
  }else
	{
		return pdFAIL;
	}
}


/**
  * @brief  将待发送的数据放置串口发送队列
  * @param  MessageId: 队列名称
  * @param  cOutChar:  发送的值
  * @param  xBlockTime: 等待时间
  * @retval uint8_t: 成功失败标识 pdPASS/pdFAIL
  */
uint8_t xSerialPutChar( osMessageQId MessageId, uint8_t cOutChar, TickType_t xBlockTime )
{
  uint8_t xReturn;

	if( osMessagePut(MessageId, (uint32_t)cOutChar, xBlockTime) == osOK )  // Send Message
	{
		xReturn = pdPASS;
	}
	else
	{
		xReturn = pdFAIL;
	}

	return xReturn;
}





/************************************重定向打印输出函数****************************************/
/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
  /* 推送到打印队列 */
  xSerialPutChar( PRINT_QUEUES_HANDLE, ch, 0/*osWaitForever*/ );
  
//  /* 将Printf内容发往串口 */
//  BSP_USART_SendData( &BSP_USART_PRINT, ch );

	return (ch);
}



/*
 * 函数名：getc
 * 描述  ：重定向c库函数getc到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由getchar调用
 */
int getc(FILE * f)
{
  return (int)BSP_USART_ReceiveData( &BSP_USART_PRINT );
}




/********************************SPI总线操作**********************************/

 

/**
  * @brief  SPI 错误处理函数
  * @retval 无
  */
void SPIx_Error (void)
{
  /* 反初始化SPI总线 */
  HAL_SPI_DeInit(&BSP_SPI_HANDLE);

  /* 重新初始化SPI总线 */

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi1);

}


/**
  * @brief  SPI 从器件读取 4 bytes
  * @retval 读到的数据
*/
uint32_t SPIx_Read(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t readvalue = 0;
  uint32_t writevalue = 0xFFFFFFFF;
  
  status = HAL_SPI_TransmitReceive(&BSP_SPI_HANDLE, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, SpixTimeout);
  
  /* 检查通信状态标识 */
  if(status != HAL_OK)
  {
    /* 执行超时回调函数 */
    SPIx_Error();
  }

  return readvalue;
}

/**
  * @brief  SPI 写1byte到器件
  * @param  Value: 写入的值
  * @retval 无
  */
void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&BSP_SPI_HANDLE, (uint8_t*) &Value, 1, SpixTimeout);

  /* 检查通信状态 */
  if(status != HAL_OK)
  {
    /* 执行超时回调函数 */
    SPIx_Error();
  }
}
 
/**
  * @}
  */ 

/*****************************END OF FILE****/
