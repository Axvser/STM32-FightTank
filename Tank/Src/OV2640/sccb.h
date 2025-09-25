#ifndef __SCCB_H
#define __SCCB_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "bitband.h"

//////////////////	 
// OV2640 ���Ž���
//////////////////
// SCL - PA1  ��
// SDA - PA3  ��
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

//IO��������
#define SCCB_SDA_IN()  {GPIOD->MODER&=~(0<<(1*2));GPIOD->MODER|=0<<1*2;}	//PA1 ����
#define SCCB_SDA_OUT() {GPIOD->MODER&=~(0<<(1*2));GPIOD->MODER|=1<<1*2;} 	//PA1 ���

//IO��������	 
#define SCCB_SCL    		PAout(1)	 	//SCL
#define SCCB_SDA    		PAout(3) 		//SDA	 

#define SCCB_READ_SDA    	PAin(3)  		//����SDA    
#define SCCB_ID   			0X60  			//OV2640��ID

/// @brief ��ʼ��SCL/SDA��Ӧ����
void SCCB_Init(void);

/// @brief ����SCCB��ʼ�ź�
void SCCB_Start(void);

/// @brief ����SCCBֹͣ�ź�
void SCCB_Stop(void);

/// @brief ����SCCB��Ӧ���ź�
void SCCB_No_Ack(void);

/// @brief ��SCCB����д��һ���ֽ�
/// @param dat Ҫд��������ֽ�
/// @return ����1��ʾд��ɹ���0��ʾʧ��
uint8_t SCCB_WR_Byte(uint8_t dat);

/// @brief ��SCCB���߶�ȡһ���ֽ�
/// @return ���ض�ȡ���������ֽ�
uint8_t SCCB_RD_Byte(void);

/// @brief ��ָ���Ĵ���д������
/// @param reg �Ĵ�����ַ
/// @param data Ҫд�������
/// @return ����1��ʾд��ɹ���0��ʾʧ��
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data);

/// @brief ��ָ���Ĵ�����ȡ����
/// @param reg �Ĵ�����ַ
/// @return ���ض�ȡ��������
uint8_t SCCB_RD_Reg(uint8_t reg);

#endif
