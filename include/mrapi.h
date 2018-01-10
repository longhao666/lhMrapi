﻿#ifndef _MRAPI_H_
#define _MRAPI_H_
#include <stdint.h>

#ifndef _WINDOWS
#define __stdcall
#endif
#define LOG_MSG_ON
#define LOG_LEVEL 1
#define LOG_APPEND 0
#define MAX_CAN_DEVICES 4

////* JOINTS *///
#define MAX_JOINTS 14
#define MAX_BUFS 128
#define WARNING_BUFS 20

//模块类型宏定义
#define MODEL_TYPE_M14        0x010
#define MODEL_TYPE_M17        0x020
#define MODEL_TYPE_M17V2      0x021
#define MODEL_TYPE_M20        0x030
#define MODEL_TYPE_M20V2      0x031
#define MODEL_TYPE_M20E       0x031
#define MODEL_TYPE_LIFT       0x040

//驱动器模式定义
#define MODE_OPEN             0       //开环模式
#define MODE_CURRENT          1       //电流模式
#define MODE_SPEED            2       //速度模式
#define MODE_POSITION         3       //位置模式
#define MODE_CYCLESYNC        4       //循环同步

// Represent the MRAPI error and status codes
//
#define MR_ERROR_OK                0x00000U  // No error
#define MR_ERROR_TIMEOUT           0x00000U  // No error
#define MR_ERROR_BUSY              0x00004U  // Bus error: an error counter reached the 'light' limit
#define MR_ERROR_ACK0              0x00008U  // Bus error: an error counter reached the 'heavy' limit
#define MR_ERROR_ACK1              0x00008U  // Bus error: an error counter reached the 'heavy' limit
#define MR_ERROR_QXMTFULL          0x00080U  // Transmit queue is full
#define MR_ERROR_QXMTEMPTY         0x00080U  // Transmit queue is full
#define MR_ERROR_ILLDATA           0x20000U  // Invalid data, function, or action

//错误字节MASK定义
#define ERROR_MASK_OVER_CURRENT   0x0001    //过流
#define ERROR_MASK_OVER_VOLTAGE   0x0002    //过压
#define ERROR_MASK_UNDER_VOLTAGE  0x0004    //欠压
#define ERROR_MASK_OVER_TEMP      0x0008    //过温
#define ERROR_MASK_BATTERY        0x0010    //编码器电池错误
#define ERROR_MASK_ENCODER        0x0020    //码盘错误
//#define ERROR_MASK_POTEN          0x0040    //电位器错误
#define ERROR_MASK_CURRENT_INIT   0x0080    //电流检测错误
//#define ERROR_MASK_FUSE           0x0100    //保险丝断开错误

#define isJointType(t) (t==MODEL_TYPE_M14)||(t==MODEL_TYPE_M17)||(t==MODEL_TYPE_M17V2)||(t==MODEL_TYPE_M20)||(t==MODEL_TYPE_M20V2) \
						||(t==MODEL_TYPE_M20E)||(t==MODEL_TYPE_LIFT)
#define isJointMode(t) (t==MODE_OPEN)||(t==MODE_CURRENT)||(t==MODE_SPEED)||(t==MODE_POSITION)||(t==MODE_CYCLESYNC)

typedef void* JOINT_HANDLE;
typedef int32_t(*jQueShortHandler_t)(JOINT_HANDLE pJoint, uint16_t len);
typedef int32_t(*jCallback_t)(uint16_t id, uint16_t index, void* args);

#ifdef __cplusplus
extern "C" {
#define _DEF_ARG =0
#else
#define _DEF_ARG
#endif
int32_t __stdcall startMaster(uint8_t masterId);
int32_t __stdcall stopMaster(uint8_t masterId);
int32_t __stdcall joinMaster(uint8_t masterId);
int32_t __stdcall setControlLoopFreq(int32_t hz);
void* __stdcall masterLoadSendFunction(uint8_t masterId);

JOINT_HANDLE __stdcall jointUp(uint16_t id, void* canSend); //construct Joint and put it in joint stack
int32_t      __stdcall jointDown(JOINT_HANDLE pJoint);          //destruct joint and remove it from joint stack
JOINT_HANDLE __stdcall jointSelect(uint16_t id);  //find joint by it's ID

void __stdcall jointStartServo(JOINT_HANDLE pJoint, jQueShortHandler_t handler);
void __stdcall jointStopServo(JOINT_HANDLE pJoint);

int32_t __stdcall jointPush(JOINT_HANDLE pJoint, uint8_t* buf);
int32_t __stdcall jointPoll(JOINT_HANDLE pJoint, uint8_t* buf);

int32_t __stdcall jointGetId(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetType(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetVoltage(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetTemp(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetBaudrate(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetCurrent(JOINT_HANDLE pJoint, uint32_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetSpeed(JOINT_HANDLE pJoint, uint32_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetPosition(JOINT_HANDLE pJoint, uint32_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetMode(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetMaxSpeed(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetMaxAcceleration(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGePositionLimit(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetCurrP(JOINT_HANDLE pJoint, uint16_t* pValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetCurrI(JOINT_HANDLE pJoint, uint16_t* iValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetSpeedP(JOINT_HANDLE pJoint, uint16_t* pValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetSpeedI(JOINT_HANDLE pJoint, uint16_t* iValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetPositionP(JOINT_HANDLE pJoint, uint16_t* pValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetPositionDs(JOINT_HANDLE pJoint, uint16_t* dsValue, int32_t timeout, jCallback_t callBack);
// 我写的
int32_t __stdcall jointGetSYS_ENABLE_DRIVER(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
JOINT_HANDLE __stdcall jointGetJoint(int32_t ID);
int32_t __stdcall jointGetTAG_OPEN_PWM(JOINT_HANDLE pJoint, int16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetTAG_CURRENT_L(JOINT_HANDLE pJoint, int32_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetTAG_SPEED_L(JOINT_HANDLE pJoint, int32_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetTAG_POSITION_L(JOINT_HANDLE pJoint, int32_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetS_POSITION_I(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetS_POSITION_D(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetS_SPEED_D(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetS_SPEED_DS(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetS_CURRENT_D(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetSYS_REDU_RATIO(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetSCP_MASK(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetSCP_REC_TIM(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointGetSCP_MASK_Addr_Value(JOINT_HANDLE pJoint, uint16_t* data, uint8_t addr, int32_t timeout, jCallback_t callBack);









// =================================================================================================

int32_t __stdcall jointSetID(JOINT_HANDLE pJoint, uint16_t id, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetBaudrate(JOINT_HANDLE pJoint, uint16_t baud, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetEnable(JOINT_HANDLE pJoint, uint16_t isEnable, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetPowerOnStatus(JOINT_HANDLE pJoint, uint16_t isEnable, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetSave2Flash(JOINT_HANDLE pJoint, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetZero(JOINT_HANDLE pJoint, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetClearError(JOINT_HANDLE pJoint, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetMode(JOINT_HANDLE pJoint, uint16_t mode, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetSpeed(JOINT_HANDLE pJoint, int32_t speed, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetPosition(JOINT_HANDLE pJoint, int32_t position, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetMaxSpeed(JOINT_HANDLE pJoint, int32_t maxspeed, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetMaxAcceleration(JOINT_HANDLE pJoint, int32_t maxacc, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetPositionLimit(JOINT_HANDLE pJoint, int32_t position_min, int32_t position_max, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetCurrP(JOINT_HANDLE pJoint, uint16_t pValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetCurrI(JOINT_HANDLE pJoint, uint16_t iValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetSpeedP(JOINT_HANDLE pJoint, uint16_t pValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetSpeedI(JOINT_HANDLE pJoint, uint16_t iValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetPositionP(JOINT_HANDLE pJoint, uint16_t pValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetPositionDs(JOINT_HANDLE pJoint, uint16_t dsValue, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetScpMask(JOINT_HANDLE pJoint, uint16_t mask, int32_t timeout, jCallback_t callBack);
// 我写的
int32_t __stdcall jointSetSYS_ENABLE_DRIVER(JOINT_HANDLE pJoint, uint16_t data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetTAG_OPEN_PWM(JOINT_HANDLE pJoint, int16_t data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetTAG_CURRENT_L(JOINT_HANDLE pJoint, int32_t data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetTAG_SPEED_L(JOINT_HANDLE pJoint, int32_t data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetTAG_POSITION_L(JOINT_HANDLE pJoint, int32_t data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetS_POSITION_I(JOINT_HANDLE pJoint, uint16_t data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetS_POSITION_D(JOINT_HANDLE pJoint, uint16_t data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetS_SPEED_D(JOINT_HANDLE pJoint, uint16_t data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetS_SPEED_DS(JOINT_HANDLE pJoint, uint16_t data, int32_t timeout, jCallback_t callBack);
int32_t __stdcall jointSetS_CURRENT_D(JOINT_HANDLE pJoint, uint16_t data, int32_t timeout, jCallback_t callBack);







#ifdef __cplusplus
}
#endif
#endif

