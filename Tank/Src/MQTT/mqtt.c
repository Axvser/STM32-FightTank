#include "mqtt.h"

void ESP01S_AT_RESET(void)
{
    Wifi_Send("AT+RST\r\n");
#if MQTT_DEBUG
    printf("AT+RST\r\n");
#endif
}

void ESP01S_AT_MODE(uint8_t mode)
{
    char buffer[32];
    sprintf(buffer, "AT+CWMODE=%d\r\n", mode);
    Wifi_Send(buffer);
#if MQTT_DEBUG
    printf(buffer);
#endif
}

void ESP01S_AT_DHCP(uint8_t enable, uint8_t mode)
{
    char buffer[32];
    sprintf(buffer, "AT+CWDHCP=%d,%d\r\n", enable, mode);
    Wifi_Send(buffer);
#if MQTT_DEBUG
    printf(buffer);
#endif
}

void ESP01S_AT_JAP(char *ssid, char *pwd)
{
    char buffer[64];
    sprintf(buffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pwd);
    Wifi_Send(buffer);
#if MQTT_DEBUG
    printf(buffer);
#endif
}

void ESP01S_AT_CFG(const char *deviceName, const char *productID, const char *token)
{
    char buffer[256];
    sprintf(buffer, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", deviceName, productID, token);
    Wifi_Send(buffer);
#if MQTT_DEBUG
    printf(buffer);
#endif
}

void ESP01S_AT_CONN(uint8_t linkID, char *host, uint16_t port)
{
    char buffer[256];
    sprintf(buffer, "AT+MQTTCONN=%d,\"%s\",%d,1\r\n", linkID, host, port);
    Wifi_Send(buffer);
#if MQTT_DEBUG
    printf(buffer);
#endif
}

void ESP01S_AT_SUB(uint8_t linkID, const char *productID, const char *deviceName, const char *topic)
{
    char buffer[128];
    sprintf(buffer, "AT+MQTTSUB=%d,\"$sys/%s/%s/thing/%s\",0\r\n", linkID, productID, deviceName, topic);
    Wifi_Send(buffer);
#if MQTT_DEBUG
    printf(buffer);
#endif
}

void ESP01S_AT_POST(uint8_t linkID, const char *productID, const char *deviceName,
                    double leftTrack,
                    double rightTrack,
                    double turretH,
                    double turretV,
                    double firePower)
{
    char buffer[512];
    sprintf(buffer,
            "AT+MQTTPUB=%d,\"$sys/%s/%s/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"LeftTrack\\\":{\\\"value\\\":%.2lf\\}\\,\\\"RightTrack\\\":{\\\"value\\\":%.2lf}\\,\\\"TurretH\\\":{\\\"value\\\":%.2lf}\\,\\\"TurretV\\\":{\\\"value\\\":%.2lf}\\,\\\"FirePower\\\":{\\\"value\\\":%.2lf}}}}\",0,0\r\n",
            linkID, productID, deviceName,
            leftTrack, rightTrack, turretH, turretV, firePower);
    Wifi_Send(buffer);
#if MQTT_DEBUG
    printf(buffer);
#endif
}

void ESP01S_AT_PUBGET(uint8_t linkID, uint8_t replyID, const char *productID, const char *deviceName,
                      double leftTrack,
                      double rightTrack,
                      double turretH,
                      double turretV,
                      double firePower)
{
    char buffer[512];
    sprintf(buffer, "AT+MQTTPUB=%d,\"$sys//%s/%s/thing/property/get_reply\",\"{\"id\":\"%d\"\\,\"title\":\"OK\"\\,\"code\":200\\,\"msg\":\"success\"\\,\"data\":{\"LeftTrack\":%.2lf\\,\"RightTrack\":%.2lf\\,\"TurretH\":%.2lf\\,\"TurretV\":%.2lf\\,\"FirePower\":%.2lf}}\",1,0\r\n",
            linkID, productID, deviceName, replyID,
            leftTrack,
            rightTrack,
            turretH,
            turretV,
            firePower);
    Wifi_Send(buffer);
#if MQTT_DEBUG
    printf(buffer);
#endif
}

void ESP01S_AT_PUBSET(uint8_t linkID, uint8_t replyID, const char *productID, const char *deviceName)
{
    char buffer[256];
    sprintf(buffer, "AT+MQTTPUB=%d,\"$sys/%s/%s/thing/property/set_reply\",\"{\"id\":\"%d\"\\,\"code\":200\\,\"msg\":\"success\"}\",1,0\r\n", linkID, productID, deviceName, replyID);
    Wifi_Send(buffer);
#if MQTT_DEBUG
    printf(buffer);
#endif
}
