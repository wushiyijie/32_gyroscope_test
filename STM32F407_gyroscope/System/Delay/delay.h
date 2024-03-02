#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"

void delay_us(uint32_t nus);
void delay_ms(int nms) ;
void SysTick_Init(u8 SYSCLK);
#endif
