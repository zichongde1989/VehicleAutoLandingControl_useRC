/**
  ******************************************************************************
  * @file    data_process.c
  * @author  zcd
  * @version V0.0.1
  * @date    2016-6-24
  * @brief   主控制函数
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
#include "data_typedef.h"
#include "cmsis_os.h"
#include "data_process.h"
#include "data_out.h"
#include "bsp.h"

#define ARM_MATH_CM3
#include <math.h>
#include "arm_math.h"

/* 绝对值函数  */
#define my_abs(x)   (((x)<0)?(-x):(x))

/**
  * @brief  比例控制结构体
  */
typedef struct gP_ControlParamStruct
{
  float p;  
}P_ControlParam_t;


/**
  * @brief  飞行器位置界定宏
  */
/* 有效半径定义。半径小于此范围内是才能启动控制，否则处于原位置等待*/
#define   DATAPROCESS_VALID_CONTROL_RADIUS          300       /*!< 起控半径，小于此半径开启控制。单位：cm */    

/* 有效高度定义。高度在此范围内是才能启动控制，否则处于原位置等待*/
#define   DATAPROCESS_VALID_CONTROL_ALTITUDE_MIN    50       /*!< 高度下限。单位：cm                      */    
#define   DATAPROCESS_VALID_CONTROL_ALTITUDE_MAX    500       /*!< 高度上限。单位：cm                      */    



/**
  * @brief  速度界定宏
  */
/* XY方向移动速度（仅用于定点模式下） */
#define   DATAPROCESS_XY_MOVE_VELOCITY_MAX        (0.80f)      /*!< 最大移动速度，单位：m/s,TODO::与飞控参数设定一致  */    
#define   DATAPROCESS_XY_MOVE_VELOCITY_LIMIT      (0.32f)      /*!< 移动速度限制，单位：m/s  */ 

/* Z方向移动速度（仅用于定点模式下） */
#define   DATAPROCESS_Z_MOVE_VELOCITY_MAX         (0.60f)      /*!< 最大移动速度，单位：m/s,TODO::与飞控参数设定一致  */    
#define   DATAPROCESS_Z_MOVE_VELOCITY_LIMIT       (0.20f)      /*!< 移动速度限制，单位：m/s  */ 



/**
  * @brief 调整参数界定宏
  */
/* X方向参数 */
#define   DATAPROCESS_X_CONTROL_VALUE_P_MIN       (0.00f)           /*!< 比例控制-P参数最小值           */
#define   DATAPROCESS_X_CONTROL_VALUE_P_MAX       (0.50f)           /*!< 比例控制-P参数最大值           */
#define   DATAPROCESS_X_CONTROL_VALUE_P_DEFAULT   (0.18f)           /*!< 比例控制-P参数默认值           */      
//TODO::细致调节DEFAULT参数

/* Y方向参数 */
#define   DATAPROCESS_Y_CONTROL_VALUE_P_MIN       (0.00f)           /*!< 比例控制-P参数最小值           */
#define   DATAPROCESS_Y_CONTROL_VALUE_P_MAX       (0.50f)           /*!< 比例控制-P参数最大值           */
#define   DATAPROCESS_Y_CONTROL_VALUE_P_DEFAULT   (0.16f)           /*!< 比例控制-P参数默认值           */      

/* Z方向控制参数 */
#define   DATAPROCESS_Z_CONTROL_VALUE_P_MIN       (0.00f)           /*!< 比例控制-P参数最小值           */
#define   DATAPROCESS_Z_CONTROL_VALUE_P_MAX       (0.40f )          /*!< 比例控制-P参数最大值           */
#define   DATAPROCESS_Z_CONTROL_VALUE_P_DEFAULT   (0.05f)           /*!< 比例控制-P参数默认值           */      




/*************************** 全局变量结构体 ****************************/
/* 比例控制结构体 */
static P_ControlParam_t*   P_ControlParam_X;
static P_ControlParam_t*   P_ControlParam_Y;
static P_ControlParam_t*   P_ControlParam_Z;

/* 保存数据处理时间戳 */
static uint32_t    gLast_Processtime;
#define   MAX_SENSOR_LOSE_TIME    800 /*!< 最大允许传感器丢失时间(传感器丢失后，数据处理响应停止)，单位:ms  */

/****************************** 函数定义 *******************************/
/**
  * @brief  初始化比例控制的结构体
  * @param  control_param: 比例参数结构体
  * @param  input: 输入值
  * @param  param_p: 设定参数初始值
  * 
  * @retval float: 控制量
  */
static void   DataProcess_InitPControlParam( P_ControlParam_t* control_param,float param_p )
{
  assert_param( control_param != NULL );
  
  /* 赋值 */
  if( param_p > DATAPROCESS_X_CONTROL_VALUE_P_MAX)
  {
    control_param->p  = DATAPROCESS_X_CONTROL_VALUE_P_MAX ;
  }else if( param_p < DATAPROCESS_X_CONTROL_VALUE_P_MIN)
  {
    control_param->p  = DATAPROCESS_X_CONTROL_VALUE_P_MIN ;
  }else
  {
    control_param->p  = param_p ;
  }

}



/**
  * @brief  判断是否在可控范围内
  * @param  pSensorData: 传感器值
  * @retval uint8_t: 判断结果。 安全区内=pdTRUE,安全区外=pdFALSE;
  */
static uint8_t    DataProcess_isInSaftyZone( SensorData_t* pSensorData )
{
  assert_param( pSensorData != NULL );
  
  /* 判断飞行器是否处于安全半径以内 */
  if( my_abs( pSensorData->x ) > DATAPROCESS_VALID_CONTROL_RADIUS )
  {
    return pdFALSE;
  }
  if( my_abs(pSensorData->y) > DATAPROCESS_VALID_CONTROL_RADIUS )
  {
    return pdFALSE;
  }
  
  /* 判断飞行器是否在安全高度中 */
  if( pSensorData->z > DATAPROCESS_VALID_CONTROL_ALTITUDE_MAX )
  {
    return pdFALSE;
  }else  if( pSensorData->z < DATAPROCESS_VALID_CONTROL_ALTITUDE_MIN )
  {
    return pdFALSE;
  }
  
  
  return pdTRUE;
}



/**
  * @brief  根据目标点和当前点的距离差，结合比例参数计算出控制值
  * @param  control_param: 比例参数结构体
  * @param  input: 输入值
  * @param  setpoint: 目标值
  * 
  * @retval float: 控制量
  */
static float   DataProcess_CalcPControlValue( P_ControlParam_t* control_param,int16_t input , int16_t  setpoint  )
{
  float  err;
  float  out;

  assert_param( control_param != NULL );
  
 
  /* 1.计算当前值与目标值的差距 */
  err = setpoint - input;
  
  /* 2.根据误差距离和比例系数的乘积输出数据 */
  out = control_param->p * err ;
 
 
  return out;
}




/****************************** 导出的函数 定义 *******************************/


/**
  * @brief  初始化控制模块
  * @param  none
  * @retval uint8_t: 执行状态
  */
uint8_t   DataProcess_Init( void )
{
  /* 为结构体申请空间 */
  P_ControlParam_X = pvPortMalloc(sizeof(P_ControlParam_t));
  P_ControlParam_Y = pvPortMalloc(sizeof(P_ControlParam_t));
  P_ControlParam_Z = pvPortMalloc(sizeof(P_ControlParam_t));
  
  /* 初始化结构体 */
  if( (P_ControlParam_X != NULL)&&(P_ControlParam_Y != NULL)&&(P_ControlParam_Z != NULL) )
  {
    DataProcess_InitPControlParam( P_ControlParam_X, DATAPROCESS_X_CONTROL_VALUE_P_DEFAULT );
    DataProcess_InitPControlParam( P_ControlParam_Y, DATAPROCESS_Y_CONTROL_VALUE_P_DEFAULT );
    DataProcess_InitPControlParam( P_ControlParam_Z, DATAPROCESS_Z_CONTROL_VALUE_P_DEFAULT );
  }
  else
  {
    SYS_DEBUG("pvPortMalloc Fail!!\n");
    return pdFAIL;
  }
  
  return pdPASS;
}




/**
  * @brief  输出中位电压，使飞行器保持位置
  * @param  pControlData: 输出电压值
  * @retval none
  */
void   DataProcess_SetDataForRelease(  ControlData_t* pControlData   )
{
  assert_param( pControlData != NULL );

  /* roll回中，使x方向停止移动 */
  pControlData->roll     = DAC_ROLL_UOUT_CENTER;
  
  /* pitch回中，使y方向停止移动 */
  pControlData->pitch    = DAC_PITCH_UOUT_CENTER;
  
  /* 油门回中，使垂直方向停止移动 */
  pControlData->throttle = DAC_THROTTLE_UOUT_CONTER;
}



/**
  * @brief   监视系统处理函数的执行时间，如果长时间没有收到数据或者没有处理，则释放飞行器 
  * @note     此函数会操作DAC，注意在其他线程使用时，与主数据输出线程不在同一时间执行
  */
void   DataProcess_CheackProcessTime(  void  )
{
  uint32_t    current_Processtime;

  /* 获取当前时间 */
  current_Processtime  = osKernelSysTick();
  
  /* 判断是否超时 */
  if(  (current_Processtime - gLast_Processtime)*portTICK_PERIOD_MS > MAX_SENSOR_LOSE_TIME )
  {
    /* 超时则释放控制 */
    ControlOut_ReleaseControl();
  }
  
}




/**
  * @brief  对传感器数据进行处理，输出控制数据
  * @param  ptr: 待校验数据指针
  * @param  len: 待校验数据长度
  *                     
  *                 (Y) [pitch]    
  *                  ^                                   
  *                  |                                   
  *                  |                                  
  *           <------0-----> (X) [roll]                              
  *                  |                                  
  *                  |                                  
  *                  V                                  
  *                                                     
  * @retval uint8_t: 校验值
  */
uint8_t   DataProcess_DoProcess( SensorData_t* pSensorData, ControlData_t* pControlData  )
{
  /* 储存比例调节结果 */
  float XV_POut;
  float YV_POut;
  float ZV_POut;

  /* 储存按照机体姿态的分配结果*/
  float XV_Out;
  float YV_Out;
  float ZV_Out;
  
  float dYaw;       /* 机体坐标相对于局部坐标系的旋转量 */
  dYaw = 0;         /* 机体坐标相对于局部坐标系一致时, dYaw为0*/
  
  /* 检查是否为空 */
  assert_param( pSensorData != NULL );
  assert_param( pControlData != NULL );
  
#if 0   
    SYS_DEBUG("Sensor Data=(%d,%d,%d)\n",pSensorData->x,pSensorData->y,pSensorData->z);
#endif

  /* 执行控制 */
  if(  DataProcess_isInSaftyZone( pSensorData ) == pdTRUE )
  {
    
    /* 1.根据当前位置、目标位置，计算出朝目标移动的速度控制量  */
    XV_POut = DataProcess_CalcPControlValue( P_ControlParam_X  , pSensorData->x , 0 );       // x方向控制，通过roll 通道控制x方向
    YV_POut = DataProcess_CalcPControlValue( P_ControlParam_Y  , pSensorData->y , 0 );       // y方向控制，通过pitch通道控制y方向
    ZV_POut = DataProcess_CalcPControlValue( P_ControlParam_Z  , pSensorData->z , 0 );       // z方向控制，通过Throttle通道控制z方向

#if 0   
  SYS_DEBUG("XV_POut=%f,YV_POut=%f,ZV_POut=%f\n",XV_POut,YV_POut,ZV_POut);
#endif
 
    /* 单位转换：cm--->m    */
    XV_POut = XV_POut/100.0;
    YV_POut = YV_POut/100.0;
    ZV_POut = ZV_POut/100.0;
    
    /* 2.将控制量分配到 机体坐标系的 X,Y,Z（对应Roll、Pitch、Throttle） 方向 */
    XV_Out  = +XV_POut * cos(dYaw) - YV_POut*sin(dYaw) ;
    YV_Out  = +XV_POut * sin(dYaw) + YV_POut*cos(dYaw) ;
    ZV_Out  = ZV_POut;
    
#if 0   
    SYS_DEBUG("XV_Out  = +%f * cos(%f) - %f*sin(%f) = %f ;\n",XV_POut,dYaw, YV_POut,dYaw,XV_Out );
    SYS_DEBUG("YV_Out  = +%f * sin(%f) + %f*cos(%f) = %f ;\n",XV_POut,dYaw, YV_POut,dYaw,YV_Out );
    SYS_DEBUG("ZV_Out  = %f\n", ZV_Out );
#endif
    
    /* 范围限制  */
    if( XV_Out > +DATAPROCESS_XY_MOVE_VELOCITY_LIMIT )   { XV_Out = +DATAPROCESS_XY_MOVE_VELOCITY_LIMIT;  }
    if( XV_Out < -DATAPROCESS_XY_MOVE_VELOCITY_LIMIT )   { XV_Out = -DATAPROCESS_XY_MOVE_VELOCITY_LIMIT;  }
    if( YV_Out > +DATAPROCESS_XY_MOVE_VELOCITY_LIMIT )   { YV_Out = +DATAPROCESS_XY_MOVE_VELOCITY_LIMIT;  }
    if( YV_Out < -DATAPROCESS_XY_MOVE_VELOCITY_LIMIT )   { YV_Out = -DATAPROCESS_XY_MOVE_VELOCITY_LIMIT;  }
    if( ZV_Out > +DATAPROCESS_Z_MOVE_VELOCITY_LIMIT  )   { ZV_Out = +DATAPROCESS_Z_MOVE_VELOCITY_LIMIT ;  }
    if( ZV_Out < -DATAPROCESS_Z_MOVE_VELOCITY_LIMIT  )   { ZV_Out = -DATAPROCESS_Z_MOVE_VELOCITY_LIMIT ;  }
    
    /* 3.将控制量折算成控制电压 */
    pControlData->roll      = (XV_Out/DATAPROCESS_XY_MOVE_VELOCITY_MAX)*DAC_ROLL_UOUT_CONTROL_ZONE*DAC_ROLL_UOUT_DIRECTIOPN\
                              + DAC_ROLL_UOUT_CENTER    ;
    pControlData->pitch     = (YV_Out/DATAPROCESS_XY_MOVE_VELOCITY_MAX)*DAC_PITCH_UOUT_CONTROL_ZONE*DAC_PITCH_UOUT_DIRECTIOPN\
                              + DAC_PITCH_UOUT_CENTER   ;
    pControlData->throttle  = (ZV_Out/DATAPROCESS_Z_MOVE_VELOCITY_MAX )*DAC_THROTTLE_UOUT_CONTROL_ZONE*DAC_THROTTLE_UOUT_DIRECTIOPN\
                              + DAC_THROTTLE_UOUT_CONTER;  
    
    
    
    /* 输出数据 */
#if 1   
    SYS_DEBUG("roll     = (%f/%f)*%f*%d+%f=%f\n", XV_Out, DATAPROCESS_XY_MOVE_VELOCITY_MAX, DAC_ROLL_UOUT_CONTROL_ZONE    ,DAC_ROLL_UOUT_DIRECTIOPN    ,DAC_ROLL_UOUT_CENTER    , pControlData->roll);
    SYS_DEBUG("pitch    = (%f/%f)*%f*%d+%f=%f\n", YV_Out, DATAPROCESS_XY_MOVE_VELOCITY_MAX, DAC_PITCH_UOUT_CONTROL_ZONE   ,DAC_PITCH_UOUT_DIRECTIOPN   ,DAC_PITCH_UOUT_CENTER   , pControlData->pitch);
    SYS_DEBUG("throttle = (%f/%f)*%f*%d+%f=%f\n", ZV_Out, DATAPROCESS_Z_MOVE_VELOCITY_MAX, DAC_THROTTLE_UOUT_CONTROL_ZONE ,DAC_THROTTLE_UOUT_DIRECTIOPN,DAC_THROTTLE_UOUT_CONTER , pControlData->throttle);
#endif
    
    /* 刷新处理时间戳 */
    gLast_Processtime = osKernelSysTick();
  
  }else
  {
    /* 填写中位数值，使飞行器保持不动 */
    DataProcess_SetDataForRelease( pControlData );
    
    /* 输出提示 */
    SYS_DEBUG("Not in safety area, holding...\n");

  }
  
  
  return 0;
}

