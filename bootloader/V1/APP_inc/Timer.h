#ifndef _TIMER_H
#define _TIMER_H


#include"Include.h"


void    isr_13us(void);

void    timer2_init(void);

void    timer2_enable(void);

void    timer2_disable(void);

void    Enable_SysTick(void);

void    Disable_SysTick(void);



void TIM3_NVIC_Configuration(void);

void USB_OTG_BSP_mDelay (const uint32_t msec);
void USB_OTG_BSP_uDelay (const uint32_t usec);

#endif


