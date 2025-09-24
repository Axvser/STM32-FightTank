#include "stm32f4xx.h"
#include "motor.h"
#include "gun.h"
#include "wifi.h"
#include "mqtt.h"
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

void Test_Wifi(void)
{
    Wifi_Init_AP(115200,"STM32-Tank","88888888","192.168.5.1","8080","255.255.255.0");
    MotorContext motor = Motor_Init(1679, 0);
    GunContext gun = Gun_Init(999, 0);
    while (1)
    {
        Wifi_FrameUpdate(&motor,&gun);
    }
}

void Test_Tank(void)
{
    MotorContext motor = Motor_Init(1679, 0);
    GunContext gun = Gun_Init(999, 0);
    Test_GunFire(&gun, 1);
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

void Test_MQTT(void)
{
    Wifi_Init(115200);
    ESP01S_AT_RESET();
    delay_ms(500);
    ESP01S_AT_MODE(1);
    delay_ms(500);
    ESP01S_AT_DHCP(1, 1);
    delay_ms(500);
    ESP01S_AT_JAP("Axvser", "888888881");
    delay_ms(2000);
    ESP01S_AT_CFG(MQTT_DEV_NAME, MQTT_DEV_ID, MQTT_TOKEN);
    delay_ms(500);
    ESP01S_AT_CONN(MQTT_LINK_ID, MQTT_HOST, MQTT_PORT);
    delay_ms(1000);
    ESP01S_AT_POST(MQTT_LINK_ID, MQTT_DEV_ID, MQTT_DEV_NAME, 0.2, 0.2, 4, 3, 0.8);
    delay_ms(300);
}
