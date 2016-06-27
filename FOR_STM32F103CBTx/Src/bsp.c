/**
  ******************************************************************************
  * @file    bsp.c
  * @author  �ֳ��
  * @version V1.0.0
  * @date    2015-11-12
  * @brief   �弶��Դ����
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


/** @defgroup  ȫ�ֱ���
  * @{
  */ 
uint32_t SpixTimeout = SPIx_TIMEOUT_MAX;           /*<! Value of Timeout when SPI communication fails */

/**
  * @}
  */ 




/** @defgroup  ��������
  * @{
  */ 

/*************************************************************
* @brief BSP_USART_SendData ���ڷ����ַ�
* @param ch:�������ַ�
* @arg
* @retval ���ͳɹ���־λ
**************************************************************/
HAL_StatusTypeDef BSP_USART_SendData(UART_HandleTypeDef *huart, uint8_t ch)
{
 
  /*�ȴ��ϴη������*/
  while( (HAL_UART_STATE_BUSY_TX == HAL_UART_GetState(huart) )||(HAL_UART_STATE_BUSY_TX_RX == HAL_UART_GetState(huart)) );  

  /*��ʼ��������-������ʽ*/
  if( HAL_UART_Transmit(huart, &ch, 1,TRANSMIT_OUTTIME) == HAL_UART_STATE_TIMEOUT )
    return HAL_TIMEOUT; /*���ͳ�ʱ*/

  return HAL_OK;
}


/*************************************************************
* @brief BSP_USART_SendArray ���ڷ����ַ���
* @param ptr:���������飬size�����͵��ֽ���
* @arg
* @retval ����״̬��ʶ
**************************************************************/
HAL_StatusTypeDef BSP_USART_SendArray(UART_HandleTypeDef *huart,  uint8_t* ptr,uint16_t size)
{
  if(NULL == ptr)
    return HAL_ERROR;
  

  if(size)
  {
    /*�ȴ��ϴη������*/
    while( (HAL_UART_STATE_BUSY_TX == HAL_UART_GetState(huart) )||(HAL_UART_STATE_BUSY_TX_RX == HAL_UART_GetState(huart)) );  

    
    /*��ʼ��������-������ʽ*/
    if( HAL_UART_Transmit(huart, ptr, size,TRANSMIT_OUTTIME) == HAL_UART_STATE_TIMEOUT )
      return HAL_TIMEOUT; /*���ͳ�ʱ*/
  
  }

  return HAL_OK;  
}



/*************************************************************
* @brief BSP_USART_SendData ���ڽ���һ���ַ�
* @param 
* @arg
* @retval ch: ���ؽ��յ����ַ�
**************************************************************/
uint8_t BSP_USART_ReceiveData(UART_HandleTypeDef *huart )
{
  uint8_t ch;
 
  /*�ȴ��ϴν������*/
  while( (HAL_UART_STATE_BUSY_RX == HAL_UART_GetState(huart) )||(HAL_UART_STATE_BUSY_TX_RX  == HAL_UART_GetState(huart)) );  

  /*��ʼ��������*/
  if( HAL_UART_STATE_TIMEOUT  ==  HAL_UART_Receive(huart, &ch, 1,RECEIVE_OUTTIME) )
    return 0;//HAL_TIMEOUT; /*���ճ�ʱ������0*/
 
  return ch;
}



/*************************************************************
* @brief BSP_USART_ReceiveArray ���ڷ����ַ�
* @param huart�������ptr:��������飬size�����͵��ֽ���
* @arg
* @retval ����״̬��ʶ
**************************************************************/
HAL_StatusTypeDef BSP_USART_ReceiveArray(UART_HandleTypeDef *huart,  uint8_t* ptr,uint16_t size)
{
  if(NULL == ptr)
    return HAL_ERROR;
  
  if(size)
  {
    /*�ȴ��ϴν������*/
    while( (HAL_UART_STATE_BUSY_RX == HAL_UART_GetState(huart) )||(HAL_UART_STATE_BUSY_TX_RX  == HAL_UART_GetState(huart)) );  

    /*��ʼ��������*/
    if(HAL_UART_STATE_TIMEOUT  ==  HAL_UART_Receive(huart, ptr, size ,RECEIVE_OUTTIME) )
      return HAL_TIMEOUT;//; /*���ճ�ʱ������0*/
    
  }

  return HAL_OK;  
}



/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART1_printf()����
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
 * ��������USART1_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���1����USART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART_printf( USART1, "\r\n this is a demo \r\n" );
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

	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
        if ( *Data == 0x5c )  //'\'
        {									  
            switch ( *++Data )
            {
                case 'r':							          //�س���
                    BSP_USART_SendData(huart,0x0d);
                    Data ++;
                break;

                case 'n':							          //���з�
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
                case 's':										  //�ַ���
                    s = va_arg(ap, const char *);
                    for ( ; *s; s++) 
                    {
                       BSP_USART_SendData(huart,*s);
                    }
                    Data++;
                    break;

                case 'd':										//ʮ����
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

/************************************���ڶ��в�������***************************************/

/**
  * @brief  �Ӵ��ڽ��ն����л�ȡ����
  * @param  MessageId: ��������
  * @param  pcRxedChar: ָ��Ŀ�괢��λ��
  * @param  xBlockTime: �ȴ�ʱ��
  * @retval uint8_t: �ɹ�ʧ�ܱ�ʶ pdPASS/pdFAIL
  */
uint8_t xSerialGetChar( osMessageQId MessageId, uint8_t *pcRxedChar, TickType_t xBlockTime )
{
  osEvent  evt;
  
  /* ������ʾ���� */
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
  * @brief  �������͵����ݷ��ô��ڷ��Ͷ���
  * @param  MessageId: ��������
  * @param  cOutChar:  ���͵�ֵ
  * @param  xBlockTime: �ȴ�ʱ��
  * @retval uint8_t: �ɹ�ʧ�ܱ�ʶ pdPASS/pdFAIL
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





/************************************�ض����ӡ�������****************************************/
/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
int fputc(int ch, FILE *f)
{
  /* ���͵���ӡ���� */
  xSerialPutChar( PRINT_QUEUES_HANDLE, ch, 0/*osWaitForever*/ );
  
//  /* ��Printf���ݷ������� */
//  BSP_USART_SendData( &BSP_USART_PRINT, ch );

	return (ch);
}



/*
 * ��������getc
 * ����  ���ض���c�⺯��getc��USART1
 * ����  ����
 * ���  ����
 * ����  ����getchar����
 */
int getc(FILE * f)
{
  return (int)BSP_USART_ReceiveData( &BSP_USART_PRINT );
}




/********************************SPI���߲���**********************************/

 

/**
  * @brief  SPI ��������
  * @retval ��
  */
void SPIx_Error (void)
{
  /* ����ʼ��SPI���� */
  HAL_SPI_DeInit(&BSP_SPI_HANDLE);

  /* ���³�ʼ��SPI���� */

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
  * @brief  SPI ��������ȡ 4 bytes
  * @retval ����������
*/
uint32_t SPIx_Read(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t readvalue = 0;
  uint32_t writevalue = 0xFFFFFFFF;
  
  status = HAL_SPI_TransmitReceive(&BSP_SPI_HANDLE, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, SpixTimeout);
  
  /* ���ͨ��״̬��ʶ */
  if(status != HAL_OK)
  {
    /* ִ�г�ʱ�ص����� */
    SPIx_Error();
  }

  return readvalue;
}

/**
  * @brief  SPI д1byte������
  * @param  Value: д���ֵ
  * @retval ��
  */
void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&BSP_SPI_HANDLE, (uint8_t*) &Value, 1, SpixTimeout);

  /* ���ͨ��״̬ */
  if(status != HAL_OK)
  {
    /* ִ�г�ʱ�ص����� */
    SPIx_Error();
  }
}
 
/**
  * @}
  */ 

/*****************************END OF FILE****/
