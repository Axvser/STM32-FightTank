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

// 电机测试组 [ 通过 ]
void Test_DirectMove(MotorContext *motor, double v);
void Test_TurnMove(MotorContext *motor, double v_l, double v_r);
void Test_StopMove(MotorContext *motor);

// 攻击测试组 [ 未测试 ]
void Test_GunFire(GunContext *gun, double fire);
void Test_GunRotate(GunContext *gun, double a_h, double a_v);

// Wifi测试组 [ 通过 ]
void Test_Wifi(void);

// OV2640测试组 [ 未测试 ]
// 定义全局变量
uint8_t jpeg_buffer1[JPEG_BUF_SIZE];
uint8_t jpeg_buffer2[JPEG_BUF_SIZE];
volatile uint32_t capture_count = 0;
// 声明DCMI相关函数
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
    uint8_t ret;
    uint32_t jpeg_size = 0;
    uint8_t *jpeg_data = NULL;

    printf("=== OV2640 Camera Test Started ===\r\n");

    // 1. 初始化OV2640
    printf("Initializing OV2640...\r\n");
    ret = OV2640_Init();
    // if (ret != 0)
    // {
    //     printf("OV2640 Init Failed! Error: %d\r\n", ret);
    //     return;
    // }
    printf("OV2640 Init Success!\r\n");

    // 2. 设置JPEG模式
    printf("Setting JPEG mode...\r\n");
    OV2640_JPEG_Mode();
    printf("JPEG mode set!\r\n");

    // 3. 设置800 * 600分辨率
    printf("Setting resolution to 800x600...\r\n");
    ret = OV2640_OutSize_Set(800, 600);
    if (ret != 0)
    {
        printf("Set resolution failed! Error: %d\r\n", ret);
        return;
    }
    printf("Resolution set to 800x600!\r\n");

    // 4. 初始化DCMI
    printf("Initializing DCMI...\r\n");
    My_DCMI_Init();

    // 5. 配置DMA双缓冲
    printf("Configuring DMA double buffer...\r\n");
    DCMI_DMA_Init((uint32_t)jpeg_buffer1, (uint32_t)jpeg_buffer2,
                  JPEG_BUF_SIZE / 4, DMA_MemoryDataSize_Word, DMA_MemoryInc_Enable);
    printf("DMA configured!\r\n");

    // 6. 开始捕获
    printf("Starting capture...\r\n");
    DCMI_Start();
    printf("Capture started!\r\n\n");

    // 7. 主测试循环
    while (1)
    {
        printf("Waiting for 5 seconds...\r\n");

        // 简单延时5秒
        for (uint32_t i = 0; i < 5; i++)
        {
            printf("%d... ", 5 - i);
            delay_ms(1000);
        }
        printf("\r\n");

        // 检查是否有JPEG数据就绪（使用DCMI模块的函数）
        jpeg_data = DCMI_Get_JPEG_Data(&jpeg_size);

        if (jpeg_data != NULL && jpeg_size > 100)
        {
            capture_count++;

            printf("\r\n=== Capture #%lu ===\r\n", capture_count);
            printf("JPEG Image Captured!\r\n");
            printf("Data size: %lu bytes\r\n", jpeg_size);
            printf("Buffer address: 0x%08X\r\n", (uint32_t)jpeg_data);

            // 检查JPEG文件头尾标记
            printf("Start marker: 0x%02X%02X ", jpeg_data[0], jpeg_data[1]);
            if (jpeg_data[0] == 0xFF && jpeg_data[1] == 0xD8)
                printf("(Valid JPEG start)\r\n");
            else
                printf("(Invalid start)\r\n");

            printf("End marker: 0x%02X%02X ",
                   jpeg_data[jpeg_size - 2], jpeg_data[jpeg_size - 1]);
            if (jpeg_data[jpeg_size - 2] == 0xFF && jpeg_data[jpeg_size - 1] == 0xD9)
                printf("(Valid JPEG end)\r\n");
            else
                printf("(Invalid end)\r\n");

            // 输出图像信息摘要
            printf("Image info: \r\n");

            // 查找SOF标记（Start of Frame）
            for (uint32_t i = 2; i < jpeg_size - 4; i++)
            {
                if (jpeg_data[i] == 0xFF && jpeg_data[i + 1] == 0xC0)
                {
                    uint16_t height = (jpeg_data[i + 5] << 8) | jpeg_data[i + 6];
                    uint16_t width = (jpeg_data[i + 7] << 8) | jpeg_data[i + 8];
                    uint8_t components = jpeg_data[i + 9];

                    printf("  Resolution: %ux%u\r\n", width, height);
                    printf("  Color components: %u\r\n", components);
                    break;
                }
            }

            // 输出前32字节的十六进制数据
            printf("First 32 bytes (hex):\r\n");
            for (uint32_t i = 0; i < 32 && i < jpeg_size; i++)
            {
                printf("%02X ", jpeg_data[i]);
                if ((i + 1) % 16 == 0)
                    printf("\r\n");
            }
            printf("\r\n");

            // 重置标志
            DCMI_Reset_JPEG_Flag();
            printf("=== End of Capture #%lu ===\r\n\r\n", capture_count);
        }
        else
        {
            printf("No valid JPEG data available\r\n");
            printf("jpeg_data: 0x%08X, jpeg_size: %lu\r\n",
                   (uint32_t)jpeg_data, jpeg_size);
        }

        // 每5次捕获后显示统计信息
        if (capture_count % 5 == 0 && capture_count > 0)
        {
            printf("\r\n=== Statistics ===\r\n");
            printf("Total captures: %lu\r\n", capture_count);
            printf("==================\r\n\r\n");
        }
    }
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
