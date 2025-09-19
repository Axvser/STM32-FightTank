#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"

/// @brief ��ʼ������1����debug
/// @param baud ������
void Debug_Init(uint32_t baud);

/// @brief ���ƴ���1����ַ���
/// @param baud ���ڲ�����
void Debug_WriteLine(const char *str);

#endif
