/**
  ******************************************************************************
  * @file    mode_set.c
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-25
  * @brief   检查遥控器三段开关，设置系统的输出控制模式
  * 
  ******************************************************************************
  * @verbatim
  * 1.模式包括两种：①手动控制；②自动控制；
  *     系统通过采集遥控器三段开关的位置，来选择不同的控制模式。
  *         三段开关输出状态与模式对照为: 低电平-手动控制模式；高电平-自动控制模式
  *
  * 2.系统使用 CD4053 选择最终传送到飞行器端的数据，选择时通道与控制项的对应关系如下：
  *   PITCH     <---->  CD4053_SWITCH_B
  *   ROLL      <---->  CD4053_SWITCH_C
  *   THROTTLE  <---->  CD4053_SWITCH_A
  *
  * 3. CD4053 switch引脚为0电平时，选择输出x，为1电平时，选择输出y。
  * 本系统中硬件连接如下：
  *   RC_PITCH      -- (by)-- 引脚1 | - 1  
  *                                 <------>  CD4053_SWITCH_B
  *   DAC_PITCH     -- (bx)-- 引脚2 | - 0
  *
  *   RC_ROLL       -- (cx)-- 引脚5 | - 0  
  *                                 <------>  CD4053_SWITCH_C
  *   DAC_ROLL      -- (cy)-- 引脚3 | - 1
  *  
  *   RC_THROTTLE   -- (ax)-- 引脚12| - 0
  *                                 <------>  CD4053_SWITCH_A
  *   DAC_THROTTLE  -- (ay)-- 引脚13| - 1
  *
  * 4.依照模式设定，配置硬件规则如下：
  *  (1)全手动控制模式，利用遥控器摇杆进行控制
  *     油门手动控制 
  *     俯仰手动控制 
  *     横滚手动控制
  *  (2)方向角控制模式，利用遥控器摇杆手动控制油门，根据传感器自动控制方向
  *     油门手动控制 
  *     俯仰自动控制 
  *     横滚自动控制 
  *  (3)方向角控制模式，利用遥控器摇杆手动控制方向，根据传感器自动控制油门
  *     油门自动控制 
  *     俯仰手动控制 
  *     横滚手动控制 
  *  (4)全自动控制模式，根据传感器值对油门、俯仰、偏航进行全面控制
  *     油门自动控制 
  *     俯仰自动控制 
  *     横滚自动控制 
  ******************************************************************************
  *
  ******************************************************************************
  */ 
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "data_typedef.h"
#include "bsp.h"
#include "data_out.h"
#include "mode_set.h"



/**********************************宏定义*********************************************/



/* 读取遥控器模式选择引脚电平状态 */
#define READ_MODE_SELECT_PIN_STATE()		   HAL_GPIO_ReadPin( ModeSelect_GPIO_Port, ModeSelect_Pin )

/* 设置模式控制引脚的电平 */
#define SET_CD4053_SWITCH_A_LOW()	    HAL_GPIO_WritePin(CD4053_SWITCH_A_GPIO_Port, CD4053_SWITCH_A_Pin, GPIO_PIN_RESET)
#define SET_CD4053_SWITCH_A_HIGH()	  HAL_GPIO_WritePin(CD4053_SWITCH_A_GPIO_Port, CD4053_SWITCH_A_Pin, GPIO_PIN_SET)

#define SET_CD4053_SWITCH_B_LOW()	    HAL_GPIO_WritePin(CD4053_SWITCH_B_GPIO_Port, CD4053_SWITCH_B_Pin, GPIO_PIN_RESET)
#define SET_CD4053_SWITCH_B_HIGH()	  HAL_GPIO_WritePin(CD4053_SWITCH_B_GPIO_Port, CD4053_SWITCH_B_Pin, GPIO_PIN_SET)

#define SET_CD4053_SWITCH_C_LOW()	    HAL_GPIO_WritePin(CD4053_SWITCH_C_GPIO_Port, CD4053_SWITCH_C_Pin, GPIO_PIN_RESET)
#define SET_CD4053_SWITCH_C_HIGH()	  HAL_GPIO_WritePin(CD4053_SWITCH_C_GPIO_Port, CD4053_SWITCH_C_Pin, GPIO_PIN_SET)


/**********根据对应关系设置 *************/
/*****************************************
*   THROTTLE  <---->  CD4053_SWITCH_A    *
*   PITCH     <---->  CD4053_SWITCH_B    *
*   ROLL      <---->  CD4053_SWITCH_C    *
*   控制引脚为低电平时，选中手动模式，   *
*   反之，对应通道为自动模式             *
*   注意：                               *
*     硬件实测中，pitch选择方式与其他相反*
*****************************************/
#define SET_THROTTLE_MANUL()	  SET_CD4053_SWITCH_A_LOW()
#define SET_THROTTLE_AUTO()	    SET_CD4053_SWITCH_A_HIGH()

#define SET_PITCH_MANUL()	      SET_CD4053_SWITCH_B_HIGH() 
#define SET_PITCH_AUTO()	      SET_CD4053_SWITCH_B_LOW()

#define SET_ROLL_MANUL()	      SET_CD4053_SWITCH_C_LOW()	
#define SET_ROLL_AUTO()	        SET_CD4053_SWITCH_C_HIGH()


/********************************全局变量*********************************************/
uint8_t  gCurrentControl_Mode = CONTROL_MODE_MANUL;    //当前模式
uint8_t  gLasControl_Mode = CONTROL_MODE_MANUL;        //上一个模式

uint8_t  gCurrent_SelectPinstate ; //当前模式选择开关状态
uint8_t  gLast_SelectPinstate ;    //上一个模式选择开关状态



/********************************局部函数*********************************************/

/**
  * @brief  读取遥控器模式设置开关的状态
  * @param  无
  * @retval 无
  */
static uint8_t ModeControl_ReadModePinState( void )
{
  return READ_MODE_SELECT_PIN_STATE();
}


/**
  * @brief  选择控制模式
  * @param  无
  * @retval 无
  */
static uint8_t ModeControl_SelectMode( ControlMode_t mode)
{
  
//  ASSERT( mode< CONTROL_MODE_MAX);
  /* 依照模式设定，配置硬件 */
  switch( mode )
  {
    
    /* 全手动控制模式，利用遥控器摇杆进行控制*/
    case CONTROL_MODE_MANUL: 
      //SYS_DEBUG("CONTROL_MODE_MANUL\n" );
      SET_THROTTLE_MANUL();   /* 油门手动控制 */
      SET_PITCH_MANUL();      /* 俯仰手动控制 */
      SET_ROLL_MANUL();       /* 横滚手动控制 */
      break;
    
    /* 方向角控制模式，利用遥控器摇杆手动控制油门，根据传感器自动控制方向*/
    case CONTROL_MODE_AUTO_PITCH_ROLL: 
      //SYS_DEBUG("CONTROL_MODE_AUTO_PITCH_ROLL\n" );      
      SET_THROTTLE_MANUL();   /* 油门手动控制 */
      SET_PITCH_AUTO();       /* 俯仰自动控制 */
      SET_ROLL_AUTO();        /* 横滚自动控制 */
      break;
    
    /* 方向角控制模式，利用遥控器摇杆手动控制方向，根据传感器自动控制油门*/
    case CONTROL_MODE_AUTO_THROTTLE: 
      //SYS_DEBUG("CONTROL_MODE_AUTO_THROTTLE\n" );      
      SET_THROTTLE_AUTO();    /* 油门自动控制 */
      SET_PITCH_MANUL();      /* 俯仰手动控制 */
      SET_ROLL_MANUL();       /* 横滚手动控制 */
      break;
    
    /* 全自动控制模式，根据传感器值对油门、俯仰、偏航进行全面控制*/
    case CONTROL_MODE_AUTO_ALL: 
      //SYS_DEBUG("CONTROL_MODE_AUTO_ALL\n" );      
      SET_THROTTLE_AUTO();    /* 油门自动控制 */
      SET_PITCH_AUTO();       /* 俯仰自动控制 */
      SET_ROLL_AUTO();        /* 横滚自动控制 */
      break;
    
    default : 
      break;
  }
 
  /* 更新全局变量 */
  gLasControl_Mode = gCurrentControl_Mode ;
  gCurrentControl_Mode = mode;
  
  return 0;
}



/************************* 导出的函数定义 ****************************/
/**
  * @brief  初始化模式控制相关变量
  * @param  无
  * @retval 无
  */
uint8_t ModeControl_Init( void )
{

  /* 读取遥控器模式设置拨码开关状态 */ 
  gCurrent_SelectPinstate = ModeControl_ReadModePinState();
  gLast_SelectPinstate = gCurrent_SelectPinstate;
  
  
  /* 设置默认模式为手动模式，不管拨码开关状态 */
  gCurrentControl_Mode = CONTROL_MODE_MANUL;      //当前模式
  gLasControl_Mode = gCurrentControl_Mode;        //上一个模式
  ModeControl_SelectMode( CONTROL_MODE_MANUL);
  
 
  return 0;
}


/**
  * @brief  读取模式切换开关状态，并切换到合适模式
  * @param  无
  * @retval 无
  */
uint8_t ModeControl_CheckAndSetAutoMode( ControlMode_t auto_mode )
{
  

  /* 读取遥控器模式设置拨码开关状态 */ 
  gCurrent_SelectPinstate = ModeControl_ReadModePinState();
  
  
  /* 判断状态是否发生改变 */
  if( gCurrent_SelectPinstate != gLast_SelectPinstate )
  {
    if( MODE_MANUL_SELECT_PIN_STATE == gCurrent_SelectPinstate )
    {
      ModeControl_SelectMode( CONTROL_MODE_MANUL);
    }
    else
    {
      ModeControl_SelectMode( auto_mode );
    }
    
  }
  
  /* 记录本次开关状态 */
  gLast_SelectPinstate = gCurrent_SelectPinstate;
  
  return 0;
}



