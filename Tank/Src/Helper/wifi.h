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

/// @brief ��ʼ��WIFI����
/// @param baud
void Wifi_Init(uint32_t baud);

/// @brief ������Ϣ��WIFI����
/// @param str
void Wifi_Send(const char *str);

/// @brief .NET��λ��������AP����ʵʩTCP���ݴ���
/// @param baud AP���ڲ�����
/// @param wifiName AP����
/// @param wifiPwd AP����
/// @param ip �豸IP
/// @param port �豸�˿�
/// @param mask �豸��������
void Wifi_Init_AP(uint32_t baud,
                  const char *wifiName, char *wifiPwd,
                  const char *ip, const char *port, const char *mask);

/// @brief ��������λ�����͵�ʵʱ״̬���ݲ�����Ӳ����ʩ
/// @param motor
/// @param gun
void Wifi_FrameUpdate(MotorContext *motor, GunContext *gun);

#endif
