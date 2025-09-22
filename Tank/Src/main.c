#include "stm32f4xx.h"
#include "motor.h"
#include "gun.h"
#include "wifi.h"
#include "debug.h"
#include "delay.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 综合测试组
void Test_Tank(void);

// 电机测试组 [ ? 通过测试 ]
void Test_DirectMove(MotorContext *motor, double v);
void Test_TurnMove(MotorContext *motor, double v_l, double v_r);
void Test_StopMove(MotorContext *motor);

// 攻击测试组 [ ? 舵机未测试 ? 水弹枪PWM信号控制无效 ]
void Test_GunFire(GunContext *gun, double fire);
void Test_GunRotate(GunContext *gun, double a_h, double a_v);

// Wifi测试组 [ ? OneNet连接测试 ? 数据操作 ]
void Test_Wifi(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    Debug_Init(115200);

    Test_Wifi();

    while (1)
        ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Test_Tank(void)
{
    MotorContext motor = Motor_Init(1679, 0);
    GunContext gun = Gun_Init(999, 0);
    Test_GunFire(&gun, 0.25);
    double speed = 0;
    int reverse = 0;
    Test_DirectMove(&motor, speed);
    while (1)
    {
        speed += reverse ? -0.001 : 0.001;
        if (speed > 1)
        {
            speed = 1;
            reverse = 1;
        }
        if (speed < 0)
        {
            speed = 0;
            reverse = 0;
        }
        Test_DirectMove(&motor, speed);
        delay_ms(17);
    }
}

void Test_DirectMove(MotorContext *motor, double v)
{
    motor->vl = v;
    motor->vr = v;
    Motor_Update(motor);
}

void Test_TurnMove(MotorContext *motor, double v_l, double v_r)
{
    motor->vl = v_l;
    motor->vr = v_r;
    Motor_Update(motor);
}

void Test_StopMove(MotorContext *motor)
{
    motor->vl = 0;
    motor->vr = 0;
    Motor_Update(motor);
}

void Test_GunFire(GunContext *gun, double fire)
{
    gun->fire = fire;
    Gun_Update(gun);
}

void Test_GunRotate(GunContext *gun, double a_h, double a_v)
{
    gun->ah = a_h;
    gun->av = a_v;
    Gun_Update(gun);
}

void Test_Wifi(void)
{
    Wifi_Init(115200);
#define BUFFER_SIZE 256
    char buffer[BUFFER_SIZE] = {'\0'}; // 初始化清零

    // 1. Wi-Fi初始化
    WIFI_CMD_Reset(buffer);
    Wifi_Send(buffer);
    printf("[Real Value]\n%s\n", buffer); // 添加换行
    printf("[Target Value]\nAT+RST\r\n\n");
    memset(buffer, 0, BUFFER_SIZE); // 发送后清空
    delay_ms(1000);

    WIFI_CMD_SetMode(buffer, WIFI_MODE_STA);
    Wifi_Send(buffer);
    printf("[Real Value]\n%s\n", buffer);
    printf("[Target Value]\nAT+CWMODE=1\r\n\n");
    memset(buffer, 0, BUFFER_SIZE);
    delay_ms(400);

    WIFI_CMD_SetDHCP(buffer, WIFI_DHCP_EN, WIFI_MODE_STA);
    Wifi_Send(buffer);
    printf("[Real Value]\n%s\n", buffer);
    printf("[Target Value]\nAT+CWDHCP=1,1\r\n\n");
    memset(buffer, 0, BUFFER_SIZE);
    delay_ms(200);

    WIFI_CMD_ConnectAP(buffer, WIFI_SSID, WIFI_PSWD);
    Wifi_Send(buffer);
    printf("[Real Value]\n%s\n", buffer);
    printf("[Target Value]\nAT+CWJAP=\"Axvser\",\"888888881\"\r\n\n");
    memset(buffer, 0, BUFFER_SIZE);
    delay_ms(5000);

    // 2. MQTT连接
    MQTT_CMD_UserCfg(buffer, MQTT_LINK_ID, MQTT_DEV_NAME, MQTT_DEV_ID, MQTT_TOKEN);
    Wifi_Send(buffer);
    printf("[Real Value]\n%s\n", buffer);
    printf("[Target Value]\nAT+MQTTUSERCFG=0,1,\"WIFI\",\"q413Mr879l\",\"version=2018-10-31&res=products%%2Fq413Mr879l%%2Fdevices%%2FWIFI&et=1789801319&method=md5&sign=oCJic495I%%2Bl7ZACTpS7eSA%%3D%%3D\",0,0,\"\"\r\n\n");
    memset(buffer, 0, BUFFER_SIZE);
    delay_ms(500);

    MQTT_CMD_Connect(buffer, MQTT_LINK_ID, MQTT_HOST, MQTT_PORT, 1);
    Wifi_Send(buffer);
    printf("[Real Value]\n%s\n", buffer);
    printf("[Target Value]\nAT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n\n");
    memset(buffer, 0, BUFFER_SIZE);
    delay_ms(1000);

    // 3. 订阅主题
    char topic_get[128] = {0}, topic_set[128] = {0},topic_portreply={0};
    MQTT_TOPIC_GET(topic_get, MQTT_DEV_ID, MQTT_DEV_NAME);
    MQTT_TOPIC_SET(topic_set, MQTT_DEV_ID, MQTT_DEV_NAME);
    MQTT_TOPIC_PostReply(topic_portreply,MQTT_DEV_ID, MQTT_DEV_NAME);

    MQTT_CMD_Subscribe(buffer, MQTT_LINK_ID, topic_get, 0);
    Wifi_Send(buffer);
    printf("[Real Value]\n%s\n", buffer);
    printf("[Target Value]\nAT+MQTTSUB=0,\"$sys/q413Mr879l/WIFI/thing/property/get\",0\r\n\n");
    memset(buffer, 0, BUFFER_SIZE);
    delay_ms(200);

    MQTT_CMD_Subscribe(buffer, MQTT_LINK_ID, topic_set, 0);
    Wifi_Send(buffer);
    printf("[Real Value]\n%s\n", buffer);
    printf("[Target Value]\nAT+MQTTSUB=0,\"$sys/q413Mr879l/WIFI/thing/property/set\",0\r\n\n");
    memset(buffer, 0, BUFFER_SIZE);
    delay_ms(200);

    MQTT_CMD_Subscribe(buffer,MQTT_LINK_ID,topic_portreply,0);

    // 发布战车状态
    MQTT_CMD_Post_TankStatus(0, "q413Mr879l", "WIFI", "123", "0.7", "0.1", "90.0", "45.0", "1");

    // 响应属性获取请求
    MQTT_CMD_GetReply_TankStatus(0, "q413Mr879l", "WIFI", "456", "0.7", "0.1", "90.0", "45.0", "1");

    // 响应属性设置请求
    MQTT_CMD_SetReply_TankStatus(0, "q413Mr879l", "WIFI", "789");
}
