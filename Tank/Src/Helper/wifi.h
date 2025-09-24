#ifndef __WIFI_H_
#define __WIFI_H_

#include "stm32f4xx.h"
#include "gun.h"
#include "motor.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"
#include "string.h"
#include "debug.h"
#include "delay.h"
#include "math.h"

#define WIFI_DEBUG 1

typedef struct
{
    double LeftTrack;
    double RightTrack;
    double TurretH;
    double TurretV;
    double FirePower;
    uint8_t parsed_flags;
} TankData_t;

/// @brief 初始化WIFI串口
/// @param baud
void Wifi_Init(uint32_t baud);

/// @brief 发送消息到WIFI串口
/// @param str
void Wifi_Send(const char *str);

/// @brief .NET上位机将基于AP连接实施TCP数据传输
/// @param baud AP串口波特率
/// @param wifiName AP名称
/// @param wifiPwd AP密码
/// @param ip 设备IP
/// @param port 设备端口
/// @param mask 设备子网掩码
void Wifi_Init_AP(uint32_t baud,
                  const char *wifiName, char *wifiPwd,
                  const char *ip, const char *port, const char *mask);

/// @brief 解析由上位机发送的实时状态数据并更新硬件设施
/// @param motor
/// @param gun
void Wifi_FrameUpdate(MotorContext *motor, GunContext *gun);

#endif
