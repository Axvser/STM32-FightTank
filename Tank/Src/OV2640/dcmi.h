#ifndef _DCMI_H
#define _DCMI_H

#include "stm32f4xx.h"

/// @brief DCMI DMA接收回调函数
typedef void (*dcmi_rx_callback)(void);

extern dcmi_rx_callback jpeg_data_process; // JPEG数据处理函数

void My_DCMI_Init(void);
void DCMI_DMA_Init(uint32_t mem0addr, uint32_t mem1addr, uint16_t memsize, uint32_t memblen, uint32_t meminc);
void DCMI_Start(void);
void DCMI_Stop(void);

void DCMI_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
void DCMI_CR_Set(uint8_t pclk, uint8_t hsync, uint8_t vsync);

#endif
