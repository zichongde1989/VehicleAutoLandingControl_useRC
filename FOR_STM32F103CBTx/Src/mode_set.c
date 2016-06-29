/**
  ******************************************************************************
  * @file    mode_set.c
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-25
  * @brief   ���ң�������ο��أ�����ϵͳ���������ģʽ
  * 
  ******************************************************************************
  * @verbatim
  * 1.ģʽ�������֣����ֶ����ƣ����Զ����ƣ�
  *     ϵͳͨ���ɼ�ң�������ο��ص�λ�ã���ѡ��ͬ�Ŀ���ģʽ��
  *         ���ο������״̬��ģʽ����Ϊ: �͵�ƽ-�ֶ�����ģʽ���ߵ�ƽ-�Զ�����ģʽ
  *
  * 2.ϵͳʹ�� CD4053 ѡ�����մ��͵��������˵����ݣ�ѡ��ʱͨ���������Ķ�Ӧ��ϵ���£�
  *   PITCH     <---->  CD4053_SWITCH_B
  *   ROLL      <---->  CD4053_SWITCH_C
  *   THROTTLE  <---->  CD4053_SWITCH_A
  *
  * 3. CD4053 switch����Ϊ0��ƽʱ��ѡ�����x��Ϊ1��ƽʱ��ѡ�����y��
  * ��ϵͳ��Ӳ���������£�
  *   RC_PITCH      -- (by)-- ����1 | - 1  
  *                                 <------>  CD4053_SWITCH_B
  *   DAC_PITCH     -- (bx)-- ����2 | - 0
  *
  *   RC_ROLL       -- (cx)-- ����5 | - 0  
  *                                 <------>  CD4053_SWITCH_C
  *   DAC_ROLL      -- (cy)-- ����3 | - 1
  *  
  *   RC_THROTTLE   -- (ax)-- ����12| - 0
  *                                 <------>  CD4053_SWITCH_A
  *   DAC_THROTTLE  -- (ay)-- ����13| - 1
  *
  * 4.����ģʽ�趨������Ӳ���������£�
  *  (1)ȫ�ֶ�����ģʽ������ң����ҡ�˽��п���
  *     �����ֶ����� 
  *     �����ֶ����� 
  *     ����ֶ�����
  *  (2)����ǿ���ģʽ������ң����ҡ���ֶ��������ţ����ݴ������Զ����Ʒ���
  *     �����ֶ����� 
  *     �����Զ����� 
  *     ����Զ����� 
  *  (3)����ǿ���ģʽ������ң����ҡ���ֶ����Ʒ��򣬸��ݴ������Զ���������
  *     �����Զ����� 
  *     �����ֶ����� 
  *     ����ֶ����� 
  *  (4)ȫ�Զ�����ģʽ�����ݴ�����ֵ�����š�������ƫ������ȫ�����
  *     �����Զ����� 
  *     �����Զ����� 
  *     ����Զ����� 
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



/**********************************�궨��*********************************************/



/* ��ȡң����ģʽѡ�����ŵ�ƽ״̬ */
#define READ_MODE_SELECT_PIN_STATE()		   HAL_GPIO_ReadPin( ModeSelect_GPIO_Port, ModeSelect_Pin )

/* ����ģʽ�������ŵĵ�ƽ */
#define SET_CD4053_SWITCH_A_LOW()	    HAL_GPIO_WritePin(CD4053_SWITCH_A_GPIO_Port, CD4053_SWITCH_A_Pin, GPIO_PIN_RESET)
#define SET_CD4053_SWITCH_A_HIGH()	  HAL_GPIO_WritePin(CD4053_SWITCH_A_GPIO_Port, CD4053_SWITCH_A_Pin, GPIO_PIN_SET)

#define SET_CD4053_SWITCH_B_LOW()	    HAL_GPIO_WritePin(CD4053_SWITCH_B_GPIO_Port, CD4053_SWITCH_B_Pin, GPIO_PIN_RESET)
#define SET_CD4053_SWITCH_B_HIGH()	  HAL_GPIO_WritePin(CD4053_SWITCH_B_GPIO_Port, CD4053_SWITCH_B_Pin, GPIO_PIN_SET)

#define SET_CD4053_SWITCH_C_LOW()	    HAL_GPIO_WritePin(CD4053_SWITCH_C_GPIO_Port, CD4053_SWITCH_C_Pin, GPIO_PIN_RESET)
#define SET_CD4053_SWITCH_C_HIGH()	  HAL_GPIO_WritePin(CD4053_SWITCH_C_GPIO_Port, CD4053_SWITCH_C_Pin, GPIO_PIN_SET)


/**********���ݶ�Ӧ��ϵ���� *************/
/*****************************************
*   THROTTLE  <---->  CD4053_SWITCH_A    *
*   PITCH     <---->  CD4053_SWITCH_B    *
*   ROLL      <---->  CD4053_SWITCH_C    *
*   ��������Ϊ�͵�ƽʱ��ѡ���ֶ�ģʽ��   *
*   ��֮����Ӧͨ��Ϊ�Զ�ģʽ             *
*   ע�⣺                               *
*     Ӳ��ʵ���У�pitchѡ��ʽ�������෴*
*****************************************/
#define SET_THROTTLE_MANUL()	  SET_CD4053_SWITCH_A_LOW()
#define SET_THROTTLE_AUTO()	    SET_CD4053_SWITCH_A_HIGH()

#define SET_PITCH_MANUL()	      SET_CD4053_SWITCH_B_HIGH() 
#define SET_PITCH_AUTO()	      SET_CD4053_SWITCH_B_LOW()

#define SET_ROLL_MANUL()	      SET_CD4053_SWITCH_C_LOW()	
#define SET_ROLL_AUTO()	        SET_CD4053_SWITCH_C_HIGH()


/********************************ȫ�ֱ���*********************************************/
uint8_t  gCurrentControl_Mode = CONTROL_MODE_MANUL;    //��ǰģʽ
uint8_t  gLasControl_Mode = CONTROL_MODE_MANUL;        //��һ��ģʽ

uint8_t  gCurrent_SelectPinstate ; //��ǰģʽѡ�񿪹�״̬
uint8_t  gLast_SelectPinstate ;    //��һ��ģʽѡ�񿪹�״̬



/********************************�ֲ�����*********************************************/

/**
  * @brief  ��ȡң����ģʽ���ÿ��ص�״̬
  * @param  ��
  * @retval ��
  */
static uint8_t ModeControl_ReadModePinState( void )
{
  return READ_MODE_SELECT_PIN_STATE();
}


/**
  * @brief  ѡ�����ģʽ
  * @param  ��
  * @retval ��
  */
static uint8_t ModeControl_SelectMode( ControlMode_t mode)
{
  
//  ASSERT( mode< CONTROL_MODE_MAX);
  /* ����ģʽ�趨������Ӳ�� */
  switch( mode )
  {
    
    /* ȫ�ֶ�����ģʽ������ң����ҡ�˽��п���*/
    case CONTROL_MODE_MANUL: 
      //SYS_DEBUG("CONTROL_MODE_MANUL\n" );
      SET_THROTTLE_MANUL();   /* �����ֶ����� */
      SET_PITCH_MANUL();      /* �����ֶ����� */
      SET_ROLL_MANUL();       /* ����ֶ����� */
      break;
    
    /* ����ǿ���ģʽ������ң����ҡ���ֶ��������ţ����ݴ������Զ����Ʒ���*/
    case CONTROL_MODE_AUTO_PITCH_ROLL: 
      //SYS_DEBUG("CONTROL_MODE_AUTO_PITCH_ROLL\n" );      
      SET_THROTTLE_MANUL();   /* �����ֶ����� */
      SET_PITCH_AUTO();       /* �����Զ����� */
      SET_ROLL_AUTO();        /* ����Զ����� */
      break;
    
    /* ����ǿ���ģʽ������ң����ҡ���ֶ����Ʒ��򣬸��ݴ������Զ���������*/
    case CONTROL_MODE_AUTO_THROTTLE: 
      //SYS_DEBUG("CONTROL_MODE_AUTO_THROTTLE\n" );      
      SET_THROTTLE_AUTO();    /* �����Զ����� */
      SET_PITCH_MANUL();      /* �����ֶ����� */
      SET_ROLL_MANUL();       /* ����ֶ����� */
      break;
    
    /* ȫ�Զ�����ģʽ�����ݴ�����ֵ�����š�������ƫ������ȫ�����*/
    case CONTROL_MODE_AUTO_ALL: 
      //SYS_DEBUG("CONTROL_MODE_AUTO_ALL\n" );      
      SET_THROTTLE_AUTO();    /* �����Զ����� */
      SET_PITCH_AUTO();       /* �����Զ����� */
      SET_ROLL_AUTO();        /* ����Զ����� */
      break;
    
    default : 
      break;
  }
 
  /* ����ȫ�ֱ��� */
  gLasControl_Mode = gCurrentControl_Mode ;
  gCurrentControl_Mode = mode;
  
  return 0;
}



/************************* �����ĺ������� ****************************/
/**
  * @brief  ��ʼ��ģʽ������ر���
  * @param  ��
  * @retval ��
  */
uint8_t ModeControl_Init( void )
{

  /* ��ȡң����ģʽ���ò��뿪��״̬ */ 
  gCurrent_SelectPinstate = ModeControl_ReadModePinState();
  gLast_SelectPinstate = gCurrent_SelectPinstate;
  
  
  /* ����Ĭ��ģʽΪ�ֶ�ģʽ�����ܲ��뿪��״̬ */
  gCurrentControl_Mode = CONTROL_MODE_MANUL;      //��ǰģʽ
  gLasControl_Mode = gCurrentControl_Mode;        //��һ��ģʽ
  ModeControl_SelectMode( CONTROL_MODE_MANUL);
  
 
  return 0;
}


/**
  * @brief  ��ȡģʽ�л�����״̬�����л�������ģʽ
  * @param  ��
  * @retval ��
  */
uint8_t ModeControl_CheckAndSetAutoMode( ControlMode_t auto_mode )
{
  

  /* ��ȡң����ģʽ���ò��뿪��״̬ */ 
  gCurrent_SelectPinstate = ModeControl_ReadModePinState();
  
  
  /* �ж�״̬�Ƿ����ı� */
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
  
  /* ��¼���ο���״̬ */
  gLast_SelectPinstate = gCurrent_SelectPinstate;
  
  return 0;
}



