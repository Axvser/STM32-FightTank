#include "stm32f4xx.h"

#ifndef __GUN_H_
#define __GUN_H_

/// @brief ����ģ��������
/// @param arr TIM�Զ���װֵ
/// @param ah ˮƽŷ��ת��[0.360]
/// @param av ��ֱŷ��ת��[45,135]
/// @param fire ������С[0,1]
typedef struct
{
    uint16_t arr;
    double ah;
    double av;
    double fire;
} GunContext;

/// @brief ����ģ���ʼ��
/// @param arr �Զ���װֵ
/// @param psc ʱ��Ԥ��Ƶ��
/// @return ����ģ��������
GunContext Gun_Init(uint16_t arr, uint16_t psc);

/// @brief ���¹���ģ��״̬
/// @param context ����ģ��������
void Gun_Update(GunContext *context);

#endif
