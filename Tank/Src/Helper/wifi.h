#ifndef __WIFI_H_
#define __WIFI_H_

#include "stm32f4xx.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"
#include "string.h"
#include "debug.h"
#include "delay.h"

// ================= 静态配置 =================
#define WIFI_SSID "Axvser"
#define WIFI_PSWD "888888881"
#define WIFI_MODE_STA 1
#define WIFI_DHCP_EN 1
#define MQTT_HOST "mqtts.heclouds.com"
#define MQTT_PORT 1883
#define MQTT_LINK_ID 0
#define MQTT_DEV_ID "q413Mr879l"
#define MQTT_DEV_NAME "WIFI"
#define MQTT_TOKEN "version=2018-10-31&res=products%2Fq413Mr879l%2Fdevices%2FWIFI&et=1789801319&method=md5&sign=oCJic495I%2Bl7ZACTpS7eSA%3D%3D"

// ================= 动态配置 =================
#define MQTT_TOPIC_Post(buffer, product_id, device_name) sprintf(buffer, "$sys/%s/%s/thing/property/post", product_id, device_name)
#define MQTT_TOPIC_GET(buffer, product_id, device_name) sprintf(buffer, "$sys/%s/%s/thing/property/get", product_id, device_name)
#define MQTT_TOPIC_SET(buffer, product_id, device_name) sprintf(buffer, "$sys/%s/%s/thing/property/set", product_id, device_name)
#define MQTT_TOPIC_PostReply(buffer, product_id, device_name) sprintf(buffer, "$sys/%s/%s/thing/property/post_reply", product_id, device_name)
#define MQTT_TOPIC_GETReply(buffer, product_id, device_name) sprintf(buffer, "$sys/%s/%s/thing/property/get_reply", product_id, device_name)
#define MQTT_TOPIC_SETReply(buffer, product_id, device_name) sprintf(buffer, "$sys/%s/%s/thing/property/set_reply", product_id, device_name)

// ================= AT指令集 =================
#define WIFI_CMD_Reset(buffer) strcpy(buffer, "AT+RST\r\n")
#define WIFI_CMD_SetMode(buffer, mode) sprintf(buffer, "AT+CWMODE=%d\r\n", mode)
#define WIFI_CMD_SetDHCP(buffer, enable, mode) sprintf(buffer, "AT+CWDHCP=%d,%d\r\n", enable, mode)
#define WIFI_CMD_ConnectAP(buffer, ssid, pswd) sprintf(buffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pswd)
#define MQTT_CMD_UserCfg(buffer, link_id, client_id, username, password) sprintf(buffer, "AT+MQTTUSERCFG=%d,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", link_id, client_id, username, password)
#define MQTT_CMD_Connect(buffer, link_id, host, port, reconnect) sprintf(buffer, "AT+MQTTCONN=%d,\"%s\",%d,%d\r\n", link_id, host, port, reconnect)
#define MQTT_CMD_Subscribe(buffer, link_id, topic, qos) sprintf(buffer, "AT+MQTTSUB=%d,\"%s\",%d\r\n", link_id, topic, qos)
#define MQTT_CMD_Publish(buffer, link_id, topic, message, qos, retain) sprintf(buffer, "AT+MQTTPUB=%d,\"%s\",\"%s\",%d,%d\r\n", link_id, topic, message, qos, retain)

// ================= 战车专用指令 =================
#define MQTT_CMD_Post_TankStatus(buffer,link_id,product_id,device_name,msg_id,vl,vr,ah,av,fire) do{char _t[64],_p[384];snprintf(_t,sizeof(_t),"$sys/%s/%s/thing/property/post",product_id,device_name);snprintf(_p,sizeof(_p),"{\\\"id\\\":\\\"%d\\\"\\,\\\"params\\\":{\\\"LeftTrack\\\":{\\\"value\\\":%.2f}\\,\\\"RightTrack\\\":{\\\"value\\\":%.2f}\\,\\\"TurretH\\\":{\\\"value\\\":%.2f}\\,\\\"TurretV\\\":{\\\"value\\\":%.2f}\\,\\\"FirePower\\\":{\\\"value\\\":%d}}}",msg_id,vl,vr,ah,av,fire);if(strlen(_t)+strlen(_p)+50<sizeof(buffer)){snprintf(buffer,sizeof(buffer),"AT+MQTTPUB=%d,\"%s\",\"%s\",%d,%d\r\n",link_id,_t,_p,0,0);}else{printf("ERROR: Buffer overflow in Post_TankStatus\n");}}while(0)
#define MQTT_CMD_GetReply_TankStatus(buffer,link_id,product_id,device_name,req_id,vl,vr,ah,av,fire) do{char _t[64],_p[384];snprintf(_t,sizeof(_t),"$sys/%s/%s/thing/property/get_reply",product_id,device_name);snprintf(_p,sizeof(_p),"{\\\"id\\\":\\\"%s\\\",\\\"title\\\":\\\"OK\\\",\\\"code\\\":200,\\\"msg\\\":\\\"success\\\",\\\"data\\\":{\\\"LeftTrack\\\":%.2f,\\\"RightTrack\\\":%.2f,\\\"TurretH\\\":%.2f,\\\"TurretV\\\":%.2f,\\\"FirePower\\\":%d}}",req_id,vl,vr,ah,av,fire);if(strlen(_t)+strlen(_p)+50<sizeof(buffer)){snprintf(buffer,sizeof(buffer),"AT+MQTTPUB=%d,\"%s\",\"%s\",%d,%d\r\n",link_id,_t,_p,1,0);}else{printf("ERROR: Buffer overflow in GetReply\n");}}while(0)
#define MQTT_CMD_SetReply_TankStatus(buffer,link_id,product_id,device_name,req_id) do{char _t[64],_p[256];time_t _n=time(NULL);struct tm*_tm=localtime(&_n);snprintf(_t,sizeof(_t),"$sys/%s/%s/thing/property/set_reply",product_id,device_name);snprintf(_p,sizeof(_p),"{\\\"id\\\":\\\"%s\\\",\\\"code\\\":200,\\\"msg\\\":\\\"success\\\",\\\"timestamp\\\":\\\"%04d-%02d-%02d %02d:%02d:%02d\\\"}",req_id,_tm->tm_year+1900,_tm->tm_mon+1,_tm->tm_mday,_tm->tm_hour,_tm->tm_min,_tm->tm_sec);if(strlen(_t)+strlen(_p)+50<sizeof(buffer)){snprintf(buffer,sizeof(buffer),"AT+MQTTPUB=%d,\"%s\",\"%s\",%d,%d\r\n",link_id,_t,_p,1,0);}else{printf("ERROR: Buffer overflow in SetReply\n");}}while(0)

void Wifi_Init(uint32_t baud);
void Wifi_Send(const char *str);

#endif
