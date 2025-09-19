#ifndef  _BITBAND_H_
#define  _BITBAND_H_

#include "stm32f4xx.h"

#define PAout(x) *(volatile uint32_t *)(0x42000000 + (GPIOA_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PBout(x) *(volatile uint32_t *)(0x42000000 + (GPIOB_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PCout(x) *(volatile uint32_t *)(0x42000000 + (GPIOC_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PDout(x) *(volatile uint32_t *)(0x42000000 + (GPIOD_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PEout(x) *(volatile uint32_t *)(0x42000000 + (GPIOE_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PFout(x) *(volatile uint32_t *)(0x42000000 + (GPIOF_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PGout(x) *(volatile uint32_t *)(0x42000000 + (GPIOG_BASE + 0x14 - 0x40000000)*32 + x*4)
#define PHout(x) *(volatile uint32_t *)(0x42000000 + (GPIOH_BASE + 0x14 - 0x40000000)*32 + x*4)

#define PAin(x)	*(volatile uint32_t *)(0x42000000 + (GPIOA_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PBin(x)	*(volatile uint32_t *)(0x42000000 + (GPIOB_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PCin(x)	*(volatile uint32_t *)(0x42000000 + (GPIOC_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PDin(x)	*(volatile uint32_t *)(0x42000000 + (GPIOD_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PEin(x)	*(volatile uint32_t *)(0x42000000 + (GPIOE_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PFin(x)	*(volatile uint32_t *)(0x42000000 + (GPIOF_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PGin(x)	*(volatile uint32_t *)(0x42000000 + (GPIOG_BASE + 0x10 - 0x40000000)*32 + x*4)
#define PHin(x)	*(volatile uint32_t *)(0x42000000 + (GPIOH_BASE + 0x10 - 0x40000000)*32 + x*4)

#endif
