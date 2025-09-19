#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"

/// @brief 初始化串口1用于debug
/// @param baud 波特率
void Debug_Init(uint32_t baud);

/// @brief 控制串口1输出字符串
/// @param baud 串口波特率
void Debug_WriteLine(const char *str);

#endif
