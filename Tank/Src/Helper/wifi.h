#ifndef __WIFI_H_
#define __WIFI_H_

#include "stm32f4xx.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"
#include "string.h"
#include "debug.h"
#include "delay.h"

void Wifi_Init(uint32_t baud);

void Wifi_Send(const char *str);

#endif
