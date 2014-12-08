
#include    "SysTick.h"

#define SYS_72M             72000000
#define SYS_64M             64000000

void SysTick_Init(unsigned int ticks)
{

    if (SysTick_Config(SYS_64M / ticks))
    { 
        while (1);
    }
    Disable_SysTick();
}


void app_enroll_tick_hdl(void *isr,unsigned char index)
{
    if(index>1)
        index=1;
    hdl_5ms[index]=isr;


}

void Disable_SysTick(void)
{
     SysTick->CTRL &= (~(1<<SYSTICK_ENABLE) ); 
}
void Enable_SysTick(void)
{
    SysTick->CTRL |= ((1<<SYSTICK_ENABLE) ); 
}


