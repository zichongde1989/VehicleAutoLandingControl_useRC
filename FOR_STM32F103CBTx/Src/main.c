/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "data_typedef.h"
#include "bsp.h"
#include "data_process.h"
#include "data_out.h"
#include "mode_set.h"
#include "uart_received_data_parse.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_tx;

osThreadId MainTaskHandle;
osThreadId DataParseTaskHandle;
osThreadId DataProcessTaskHandle;
osThreadId DataOutTaskHandle;
osThreadId PrintTaskHandle;
osMessageQId ReceiveDataQueueHandle;
osMessageQId PrintQueuesHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t gUSART1_RX_TMP;                 // 用来保存usart1接收到的数据
uint8_t gUSART2_RX_TMP;                 // 用来保存usart2接收到的数据


/* 传感器采集值邮箱 */
osMailQDef( gSenorDataMail, 4 , SensorData_t );   
osMailQId  gSenorDataMail;

/* 处理结果输出邮箱 */
osMailQDef( gControlDataMail, 4 , ControlData_t );   
osMailQId  gControlDataMail;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
void StartMainTask(void const * argument);
void StartDataParseTask(void const * argument);
void StartDataProcessTask(void const * argument);
void StartDataOutTask(void const * argument);
void StartPrintTask(void const * argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();

  /* USER CODE BEGIN 2 */
  
    /* 硬件初始化开始 */
//  USART_printf( &BSP_USART_PRINT, "开始硬件初始化!\n");

  /* 初始化输出 */
  ControlOut_Init( );
  
  /*  初始化控制模式  */
  ModeControl_Init(  );
  
  /* 初始化主控模块 */
  DataProcess_Init( );
  
  /*启动循环接收DMA*/
  HAL_UART_Receive_DMA(&BSP_USART_SENSOR, (uint8_t*)&gUSART1_RX_TMP, 1);      


  /* 硬件初始化结束 */
//  USART_printf( &BSP_USART_PRINT, "硬件初始化结束 !\n");

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of MainTask */
  osThreadDef(MainTask, StartMainTask, osPriorityBelowNormal, 0, 128);
  MainTaskHandle = osThreadCreate(osThread(MainTask), NULL);

  /* definition and creation of DataParseTask */
  osThreadDef(DataParseTask, StartDataParseTask, osPriorityNormal, 0, 160);
  DataParseTaskHandle = osThreadCreate(osThread(DataParseTask), NULL);

  /* definition and creation of DataProcessTask */
  osThreadDef(DataProcessTask, StartDataProcessTask, osPriorityAboveNormal, 0, 256);
  DataProcessTaskHandle = osThreadCreate(osThread(DataProcessTask), NULL);

  /* definition and creation of DataOutTask */
  osThreadDef(DataOutTask, StartDataOutTask, osPriorityHigh, 0, 160);
  DataOutTaskHandle = osThreadCreate(osThread(DataOutTask), NULL);

  /* definition and creation of PrintTask */
  osThreadDef(PrintTask, StartPrintTask, osPriorityBelowNormal, 0, 128);
  PrintTaskHandle = osThreadCreate(osThread(PrintTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of ReceiveDataQueue */
  osMessageQDef(ReceiveDataQueue, 128, uint8_t);
  ReceiveDataQueueHandle = osMessageCreate(osMessageQ(ReceiveDataQueue), NULL);

  /* definition and creation of PrintQueues */
  osMessageQDef(PrintQueues, 512, uint8_t);
  PrintQueuesHandle = osMessageCreate(osMessageQ(PrintQueues), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

  /* 创建传感器采集值邮箱 */
  gSenorDataMail = osMailCreate(osMailQ(gSenorDataMail), NULL);     

  /* 创建处理结果输出邮箱 */
  gControlDataMail = osMailCreate(osMailQ(gSenorDataMail), NULL);     

  
  
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : ModeSelect_Pin */
  GPIO_InitStruct.Pin = ModeSelect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ModeSelect_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CD4053_SWITCH_C_Pin CD4053_SWITCH_B_Pin CD4053_SWITCH_A_Pin */
  GPIO_InitStruct.Pin = CD4053_SWITCH_C_Pin|CD4053_SWITCH_B_Pin|CD4053_SWITCH_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : DAC_CS2_Pin DAC_CS1_Pin DAC_CS0_Pin */
  GPIO_InitStruct.Pin = DAC_CS2_Pin|DAC_CS1_Pin|DAC_CS0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CD4053_SWITCH_C_Pin|CD4053_SWITCH_B_Pin|CD4053_SWITCH_A_Pin|DAC_CS2_Pin 
                          |DAC_CS1_Pin|DAC_CS0_Pin, GPIO_PIN_RESET);

}

/* USER CODE BEGIN 4 */



/**
  * @brief  接收完成回调函数
  * @param  huart: 指向串口实例.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(  USART1 == huart->Instance )
  {
      osMessagePut( ReceiveDataQueueHandle, gUSART1_RX_TMP , 0);  // 发送数据到缓冲区
  }
}

/**
  * @brief  打印任务堆栈剩余
  */
void PrintStackRest(void)
{
  
  SYS_DEBUG("任务栈警戒线:\n");
  SYS_DEBUG("MainTaskHandle:%d\n",(uint32_t)uxTaskGetStackHighWaterMark( MainTaskHandle) );
  SYS_DEBUG("DataParseTaskHandle:%d\n",(uint32_t)uxTaskGetStackHighWaterMark( DataParseTaskHandle) );
  SYS_DEBUG("DataProcessTaskHandle:%d\n",(uint32_t)uxTaskGetStackHighWaterMark( DataProcessTaskHandle) );
  SYS_DEBUG("DataOutTaskHandle:%d\n",(uint32_t)uxTaskGetStackHighWaterMark( DataOutTaskHandle) );
  SYS_DEBUG("PrintTaskHandle  :%d\n",(uint32_t)uxTaskGetStackHighWaterMark( PrintTaskHandle) );
}




/* USER CODE END 4 */

/* StartMainTask function */
void StartMainTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
  SYS_DEBUG_TRACK();
#ifdef  DEBUG_ON    
  uint32_t  l_time;
  uint32_t  c_time;
  l_time = c_time = 0;
#endif  
  /* Infinite loop */
  for(;;)
  {
    SYS_DEBUG("one loop!\n");
//    PrintStackRest();
                         
    
    /* 1.查询模式开关状态，如果是自动，则切换为指定的自动模式 */
    ModeControl_CheckAndSetAutoMode( CONTROL_MODE_AUTO_PITCH_ROLL  );
    
    /* 2.监视系统处理函数的执行时间，如果长时间没有收到数据或者没有处理，则释放飞行器 */
    DataProcess_CheackProcessTime( );
    
#ifdef  DEBUG_ON    
    /* 用于调试 */
    c_time = osKernelSysTick();
    if(  (c_time - l_time)*portTICK_PERIOD_MS > 2000 )
    {
      SensorData_t SensorData;
      SensorData.time_boot_ms = 10000;
      SensorData.x = 0100;
      SensorData.y = 0200;
      SensorData.z = 0200;
      SensorData.convariance = 1.2;
      DataParse_PutData( &SensorData );
    }
    l_time = c_time; 
#endif

    osDelay(200);
  }
  /* USER CODE END 5 */ 
}

/* StartDataParseTask function */
void StartDataParseTask(void const * argument)
{
  /* USER CODE BEGIN StartDataParseTask */
  uint8_t tmp;
  SYS_DEBUG_TRACK();
  
  /* Infinite loop */
  for(;;)
  {
    SYS_DEBUG("one loop!\n");

    /*------------------------接收并解析数据包---------------------------*/
    /* 1. 开始接收数据帧*/
     /*读取帧头1——head1 */
    xSerialGetChar( ReceiveDataQueueHandle, &tmp, osWaitForever ); 

    /*判断是否为帧头1——head1*/
    if( tmp == SENSOR_PROTOCOL_PACK_HEADER0 )                            
    {
      /*读取帧头2——head2*/
      xSerialGetChar( ReceiveDataQueueHandle, &tmp, osWaitForever );

      /*如果读到的还是帧头1，则重新读帧头2；防止重复的帧头1干扰*/
      if( tmp == SENSOR_PROTOCOL_PACK_HEADER0 )                           
      {
        /* 重新读取帧头2 */
        xSerialGetChar( ReceiveDataQueueHandle, &tmp, osWaitForever );        
      }
 
      /* 验证帧头2——head2*/
      if( tmp == SENSOR_PROTOCOL_PACK_HEADER1  )                        
      {
        /* 读取数据段长度 len */
        uint8_t len;
        xSerialGetChar( ReceiveDataQueueHandle, &len, osWaitForever );
        if( len == SENSOR_PROTOCOL_PACK_MAX_DATA_LEN )
        {
          /* 从邮箱申请空间，用来保存读取结果 */
          SensorData_t* pSensorData;
          pSensorData = (SensorData_t *) osMailAlloc( gSenorDataMail, osWaitForever);
          
          /* 读取数据段内容 */
          uint8_t* ptr = (uint8_t*)pSensorData ;
          uint8_t i;
          for( i = 0; i < len; i++ )
          {
            /* 循环接收整个数据段 */
            xSerialGetChar( ReceiveDataQueueHandle, ptr++, osWaitForever ); 
          }
          
          /* 读取校验字节 */
          xSerialGetChar( ReceiveDataQueueHandle, &tmp , osWaitForever ); 
          if( tmp == doSumCheck( (uint8_t*)pSensorData, len ) )
          {
            /* 如果校验通过，则发送缓冲区到队列 */
            if( osOK != osMailPut( gSenorDataMail, pSensorData) )  
            {
              ;   //TODO: Add something here
            }
          }else
          {
            /* 释放内存空间 */
            osMailFree( gSenorDataMail, pSensorData );
          }
          
        }
      }
    }
    
//    osDelay(1);
  }
  /* USER CODE END StartDataParseTask */
}

/* StartDataProcessTask function */
void StartDataProcessTask(void const * argument)
{
  /* USER CODE BEGIN StartDataProcessTask */
  SYS_DEBUG_TRACK();

  SensorData_t* pSensorData;
  ControlData_t* pControlData;
  /* Infinite loop */
  for(;;)
  {
    SYS_DEBUG("one loop!\n");

    /* -----| 1.从邮箱接收传感器数据   |---------------------*/
    osEvent event = osMailGet( gSenorDataMail, osWaitForever);
    pSensorData = (SensorData_t *)event.value.p;
    
    
    /* -----| 2.申请输出数据邮箱 |-------------------------- */
    pControlData = (ControlData_t *) osMailAlloc(gControlDataMail, osWaitForever);    
    
    
    /* -----| 3.处理数据，处理结果存放于输出缓冲区  |--------*/
    DataProcess_DoProcess( pSensorData, pControlData  );
    
    
    /* -----| 4.发送处理结果（缓冲区）到输出邮箱 | -----------*/
    osMailPut( gControlDataMail , pControlData ) ;
   
    
    /* -----| 5.释放超声波数据缓冲区  |------------------------*/
    osMailFree( gSenorDataMail, pSensorData );
    
    
//    osDelay(1);
  }
  /* USER CODE END StartDataProcessTask */
}

/* StartDataOutTask function */
void StartDataOutTask(void const * argument)
{
  /* USER CODE BEGIN StartDataOutTask */
  SYS_DEBUG_TRACK();
  ControlData_t* pControlData;
  
  
  /* Infinite loop */
  for(;;)
  {
    SYS_DEBUG("one loop!\n");
    
    /* -----| 1.接收待输出数据 |--------------------*/
    osEvent event = osMailGet( gControlDataMail, osWaitForever);
    pControlData = (ControlData_t *)event.value.p;
    
    
    /* -----| 2.输出数据       |--------------------*/
    ControlOut_PutControlData( pControlData  );
    
   
    /* -----| 3.释放数据缓冲区 |--------------------*/
    osMailFree( gControlDataMail, pControlData );
   
    
    osDelay(1);
  }
  /* USER CODE END StartDataOutTask */
}

/* StartPrintTask function */
void StartPrintTask(void const * argument)
{
  /* USER CODE BEGIN StartPrintTask */
  uint8_t byte;
  /* Infinite loop */
  for(;;)
  {
    /* 从队列取出打印数据 */
    xSerialGetChar( PRINT_QUEUES_HANDLE, &byte, osWaitForever );
    
    /* 将Printf内容发往串口 */
    BSP_USART_SendData( &BSP_USART_PRINT, byte );
    
    osDelay(1);
  }
  /* USER CODE END StartPrintTask */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
      SYS_DEBUG_TRACK();
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,*/
    USART_printf( &BSP_USART_PRINT, "Wrong parameters value: file %s on line %d\r\n", file, line) ;
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
