#include "stm32f4xx.h"
#include "bitband.h"

#ifndef __MOTOR_H_
#define __MOTOR_H_

#define AN1 PBout(11)
#define AN2 PEout(15)
#define BN1 PBout(12)
#define BN2 PBout(10)

/// @brief 电机上下文
/// @param arr TIM自动重装值
/// @param vl 左侧速度[-1,1]
/// @param vr 右侧速度[-1,1]
typedef struct
{
    uint16_t arr;
    double vl;
    double vr;
} MotorContext;

/// @brief 电机初始化
/// @param arr 自动重装值
/// @param psc 时钟预分频数
/// @return 电机上下文
MotorContext Motor_Init(uint16_t arr, uint16_t psc);

/// @brief 更新电机状态
/// @param context 电机上下文
void Motor_Update(MotorContext *context);

#endif
