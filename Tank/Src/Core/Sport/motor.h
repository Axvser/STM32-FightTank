#include "stm32f4xx.h"
#include "bitband.h"

#ifndef __MOTOR_H_
#define __MOTOR_H_

#define AN1 PBout(11)
#define AN2 PEout(15)
#define BN1 PBout(12)
#define BN2 PBout(10)

/// @brief 指定坦克电机的运行方向
typedef enum
{
    TANK_STOP = 0,
    TANK_FORE = 1,
    TANK_BACK = 2
} TankState;

/// @brief 驱动坦克电机所需要的上下文
typedef struct
{
    uint16_t arr;
    double vx;
    double vy;
    TankState state;
} TankEngine;

/// @brief 初始化TB6612FNG模块所需要的PWM与Output
/// @param arr 自动重装值
/// @param psc 时钟预分频数
/// @return 电机上下文
TankEngine Engine_Init(uint16_t arr, uint16_t psc);

/// @brief 依据上下文更新TB6612FNG模块所需要的PWM与Output
/// @param context 
void Engine_Update(TankEngine *context);

#endif
