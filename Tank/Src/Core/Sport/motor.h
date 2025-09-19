#include "stm32f4xx.h"
#include "bitband.h"

#ifndef __MOTOR_H_
#define __MOTOR_H_

#define AN1 PBout(11)
#define AN2 PEout(15)
#define BN1 PBout(12)
#define BN2 PBout(10)

/// @brief ָ��̹�˵�������з���
typedef enum
{
    TANK_STOP = 0,
    TANK_FORE = 1,
    TANK_BACK = 2
} TankState;

/// @brief ����̹�˵������Ҫ��������
typedef struct
{
    uint16_t arr;
    double vx;
    double vy;
    TankState state;
} TankEngine;

/// @brief ��ʼ��TB6612FNGģ������Ҫ��PWM��Output
/// @param arr �Զ���װֵ
/// @param psc ʱ��Ԥ��Ƶ��
/// @return ���������
TankEngine Engine_Init(uint16_t arr, uint16_t psc);

/// @brief ���������ĸ���TB6612FNGģ������Ҫ��PWM��Output
/// @param context 
void Engine_Update(TankEngine *context);

#endif
