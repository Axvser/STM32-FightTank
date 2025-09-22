#include "stm32f4xx.h"
#include "wifi.h"
#include "string.h"
#include "stdio.h"
#include "debug.h"

#ifndef __MQTT_H_
#define __MQTT_H_

#define MQTT_DEBUG 1

#define MQTT_HOST "mqtts.heclouds.com"
#define MQTT_PORT 1883
#define MQTT_LINK_ID 0
#define MQTT_DEV_ID "q413Mr879l"
#define MQTT_DEV_NAME "WIFI"
#define MQTT_TOKEN "version=2018-10-31&res=products%2Fq413Mr879l%2Fdevices%2FWIFI&et=1789801319&method=md5&sign=oCJic495I%2Bl7ZACTpS7eSA%3D%3D"

#define MQTT_TOPIC_POST "property/post/reply"
#define MQTT_TOPIC_GET "property/get"
#define MQTT_TOPIC_SET "property/set"

/// @brief ����ESP-01Sģ��
void ESP01S_AT_RESET(void);

/// @brief �趨ESP-01Sģ�鹤��ģʽ
/// @param mode
void ESP01S_AT_MODE(uint8_t mode);

/// @brief �趨ָ������ģʽΪָ��DHCP����״̬
/// @param enable 1��ʾ����
/// @param mode
void ESP01S_AT_DHCP(uint8_t enable, uint8_t mode);

/// @brief ����������
/// @param ssid �˻�
/// @param pwd ����
void ESP01S_AT_JAP(char *ssid, char *pwd);

/// @brief ����Ȩ����Ϣ
/// @param deviceName �豸��
/// @param productID ��ƷID
/// @param token ��������
void ESP01S_AT_CFG(const char *deviceName, const char *productID, const char *token);

/// @brief ������MQTT������
/// @param linkID ����ID
/// @param host ����
/// @param port �˿�
void ESP01S_AT_CONN(uint8_t linkID, char *host, uint16_t port);

/// @brief ��������
/// @param productID ��ƷID
/// @param deviceName �豸����
/// @param topic ����
void ESP01S_AT_SUB(uint8_t linkID, const char *productID, const char *deviceName, const char *topic);

/// @brief �ϱ���Ϣ
/// @param linkID ����ID
/// @param productID ��ƷID
/// @param deviceName �豸����
/// @param leftTrack ���Ĵ�PWM�� [-1,1]
/// @param rightTrack ���Ĵ�PWM�� [-1,1]
/// @param turretH ��̨ˮƽת�� [0,360]
/// @param turretV ��̨��ֱת�� [-30,60]
/// @param firePower ��̨PWM�� [0,1]
void ESP01S_AT_POST(uint8_t linkID, const char *productID, const char *deviceName,
                    double leftTrack,
                    double rightTrack,
                    double turretH,
                    double turretV,
                    double firePower);

/// @brief ��Ӧƽ̨�·���GET����
/// @param linkID ����ID
/// @param replyID ��ϢID
/// @param productID ��ƷID
/// @param deviceName �豸����
/// @param leftTrack ���Ĵ�PWM�� [-1,1]
/// @param rightTrack ���Ĵ�PWM�� [-1,1]
/// @param turretH ��̨ˮƽת�� [0,360]
/// @param turretV ��̨��ֱת�� [-30,60]
/// @param firePower ��̨PWM�� [0,1]
void ESP01S_AT_PUBGET(uint8_t linkID, uint8_t replyID, const char *productID, const char *deviceName,
                      double leftTrack,
                      double rightTrack,
                      double turretH,
                      double turretV,
                      double firePower);

/// @brief ��Ӧƽ̨�·���SET����
/// @param linkID ����ID
/// @param replyID ��ϢID
/// @param productID ��ƷID
/// @param deviceName �豸����
void ESP01S_AT_PUBSET(uint8_t linkID, uint8_t replyID, const char *productID, const char *deviceName);

#endif
