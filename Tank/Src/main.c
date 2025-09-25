#include "stm32f4xx.h"
#include "motor.h"
#include "gun.h"
#include "wifi.h"
#include "mqtt.h"
#include "ov2640.h"
#include "dcmi.h"
#include "malloc.h"
#include "debug.h"
#include "delay.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// µç»ú²âÊÔ×é [ Í¨¹ý ]
void Test_DirectMove(MotorContext *motor, double v);
void Test_TurnMove(MotorContext *motor, double v_l, double v_r);
void Test_StopMove(MotorContext *motor);

// ¹¥»÷²âÊÔ×é [ Î´²âÊÔ ]
void Test_GunFire(GunContext *gun, double fire);
void Test_GunRotate(GunContext *gun, double a_h, double a_v);

// Wifi²âÊÔ×é [ Í¨¹ý ]
void Test_Wifi(void);

// OV2640²âÊÔ×é [ Î´²âÊÔ ]
void Test_OV2640(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    Debug_Init(115200);

    Test_OV2640();

    while (1)
        ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Test_OV2640(void)
{

}

void Test_Wifi(void)
{
    Wifi_Init_AP(115200, "STM32-Tank", "88888888", "192.168.5.1", "8080", "255.255.255.0");
    MotorContext motor = Motor_Init(1679, 0);
    GunContext gun = Gun_Init(999, 0);
    while (1)
    {
        Wifi_FrameUpdate(&motor, &gun);
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
