#ifndef __SCCB_H
#define __SCCB_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "bitband.h"

//////////////////	 
// OV2640 引脚接线
//////////////////
// SCL - PA1  √
// SDA - PA3  √
// D0 - PA5
// D2 - PA7
// D4 - PC5
// D6 - PB1
// DCLK - PE8
// PWDN - PE10
// VSYNC - PA0
// HREF - PA2
// RST - PA4
// D1 - PA6
// D3 - PC4
// D5 - PB0
// D7 - PE7
//////////////////

//IO方向设置
#define SCCB_SDA_IN()  {GPIOD->MODER&=~(0<<(1*2));GPIOD->MODER|=0<<1*2;}	//PA1 输入
#define SCCB_SDA_OUT() {GPIOD->MODER&=~(0<<(1*2));GPIOD->MODER|=1<<1*2;} 	//PA1 输出

//IO操作函数	 
#define SCCB_SCL    		PAout(1)	 	//SCL
#define SCCB_SDA    		PAout(3) 		//SDA	 

#define SCCB_READ_SDA    	PAin(3)  		//输入SDA    
#define SCCB_ID   			0X60  			//OV2640的ID

/// @brief 初始化SCL/SDA对应引脚
void SCCB_Init(void);

/// @brief 发送SCCB起始信号
void SCCB_Start(void);

/// @brief 发送SCCB停止信号
void SCCB_Stop(void);

/// @brief 发送SCCB无应答信号
void SCCB_No_Ack(void);

/// @brief 向SCCB总线写入一个字节
/// @param dat 要写入的数据字节
/// @return 返回1表示写入成功，0表示失败
uint8_t SCCB_WR_Byte(uint8_t dat);

/// @brief 从SCCB总线读取一个字节
/// @return 返回读取到的数据字节
uint8_t SCCB_RD_Byte(void);

/// @brief 向指定寄存器写入数据
/// @param reg 寄存器地址
/// @param data 要写入的数据
/// @return 返回1表示写入成功，0表示失败
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data);

/// @brief 从指定寄存器读取数据
/// @param reg 寄存器地址
/// @return 返回读取到的数据
uint8_t SCCB_RD_Reg(uint8_t reg);

#endif
