#ifndef _DCMI_H
#define _DCMI_H

#include "stm32f4xx.h"

#define JPEG_BUF_SIZE 5*1024

/// @brief DCMI DMA接收回调函数
extern void (*dcmi_rx_callback)(void);

void My_DCMI_Init(void);
void DCMI_DMA_Init(uint32_t mem0addr, uint32_t mem1addr, uint16_t memsize, uint32_t memblen, uint32_t meminc);
void DCMI_Start(void);
void DCMI_Stop(void);

uint8_t* DCMI_Get_JPEG_Data(uint32_t* size);
void DCMI_Reset_JPEG_Flag(void);

void DCMI_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
void DCMI_CR_Set(uint8_t pclk, uint8_t hsync, uint8_t vsync);

#endif
