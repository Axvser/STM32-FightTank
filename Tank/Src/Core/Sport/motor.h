#include "stm32f4xx.h"
#include "bitband.h"

#ifndef __MOTOR_H_
#define __MOTOR_H_

#define AN1 PBout(11)
#define AN2 PEout(15)
#define BN1 PBout(12)
#define BN2 PBout(10)

/// @brief ���������
/// @param arr TIM�Զ���װֵ
/// @param vl ����ٶ�[-1,1]
/// @param vr �Ҳ��ٶ�[-1,1]
typedef struct
{
    uint16_t arr;
    double vl;
    double vr;
} MotorContext;

/// @brief �����ʼ��
/// @param arr �Զ���װֵ
/// @param psc ʱ��Ԥ��Ƶ��
/// @return ���������
MotorContext Motor_Init(uint16_t arr, uint16_t psc);

/// @brief ���µ��״̬
/// @param context ���������
void Motor_Update(MotorContext *context);

#endif
