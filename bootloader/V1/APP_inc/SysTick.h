#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "Include.h"

#define TICKS_5MS                   (200)   
#define TICKS_1MS                   (1000)   
#define TICKS_100US                 (10000)  
#define TICKS_13US                  (10000000/131)  //13.1us
#define TICKS_10US                  (100000) 
#define TICKS_1US                   (1000000)


void    SysTick_Init(unsigned int ticks);

void    app_enroll_tick_hdl(void *isr,unsigned char index);

void    Enable_SysTick(void);

void    Disable_SysTick(void);

#endif 


