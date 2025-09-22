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

/// @brief 重置ESP-01S模块
void ESP01S_AT_RESET(void);

/// @brief 设定ESP-01S模块工作模式
/// @param mode
void ESP01S_AT_MODE(uint8_t mode);

/// @brief 设定指定工作模式为指定DHCP启用状态
/// @param enable 1表示启用
/// @param mode
void ESP01S_AT_DHCP(uint8_t enable, uint8_t mode);

/// @brief 连接至网络
/// @param ssid 账户
/// @param pwd 密码
void ESP01S_AT_JAP(char *ssid, char *pwd);

/// @brief 配置权限信息
/// @param deviceName 设备名
/// @param productID 产品ID
/// @param token 访问令牌
void ESP01S_AT_CFG(const char *deviceName, const char *productID, const char *token);

/// @brief 连接至MQTT服务器
/// @param linkID 连接ID
/// @param host 主机
/// @param port 端口
void ESP01S_AT_CONN(uint8_t linkID, char *host, uint16_t port);

/// @brief 订阅主题
/// @param productID 产品ID
/// @param deviceName 设备名称
/// @param topic 主题
void ESP01S_AT_SUB(uint8_t linkID, const char *productID, const char *deviceName, const char *topic);

/// @brief 上报信息
/// @param linkID 连接ID
/// @param productID 产品ID
/// @param deviceName 设备名称
/// @param leftTrack 左履带PWM比 [-1,1]
/// @param rightTrack 右履带PWM比 [-1,1]
/// @param turretH 炮台水平转动 [0,360]
/// @param turretV 炮台垂直转动 [-30,60]
/// @param firePower 炮台PWM比 [0,1]
void ESP01S_AT_POST(uint8_t linkID, const char *productID, const char *deviceName,
                    double leftTrack,
                    double rightTrack,
                    double turretH,
                    double turretV,
                    double firePower);

/// @brief 响应平台下发的GET报文
/// @param linkID 连接ID
/// @param replyID 消息ID
/// @param productID 产品ID
/// @param deviceName 设备名称
/// @param leftTrack 左履带PWM比 [-1,1]
/// @param rightTrack 右履带PWM比 [-1,1]
/// @param turretH 炮台水平转动 [0,360]
/// @param turretV 炮台垂直转动 [-30,60]
/// @param firePower 炮台PWM比 [0,1]
void ESP01S_AT_PUBGET(uint8_t linkID, uint8_t replyID, const char *productID, const char *deviceName,
                      double leftTrack,
                      double rightTrack,
                      double turretH,
                      double turretV,
                      double firePower);

/// @brief 响应平台下发的SET报文
/// @param linkID 连接ID
/// @param replyID 消息ID
/// @param productID 产品ID
/// @param deviceName 设备名称
void ESP01S_AT_PUBSET(uint8_t linkID, uint8_t replyID, const char *productID, const char *deviceName);

#endif
