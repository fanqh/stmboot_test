#include "config.h"

/* --------------------------------------------------------------------
-----------------------复位和时钟控制(RCC)-----------------------
----------------------------------------------------------------------*/
#if 0
void RCC_Config(void)
{
  	ErrorStatus  HSEStartUpStatus; 

	RCC_DeInit(); 											/* reset rcc private set  */
    RCC_AdjustHSICalibrationValue(0x1); 
    RCC_HSICmd(ENABLE);

    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); /* enable flash prefetch buffer 使能预取指缓存  */		
    FLASH_SetLatency(FLASH_Latency_2); 				    /* wait 2 flash state */		
    RCC_HCLKConfig(RCC_SYSCLK_Div1);					 /* AHB clock = system clock */	
    RCC_PCLK2Config(RCC_HCLK_Div1); 					 /* APB2 clock = AHB clock */		
    RCC_PCLK1Config(RCC_HCLK_Div2); 					 /* APB1 clock = AHB clock/2 =32M*/
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16); /* PLL clock = 4MHz*16 = 64MHz */

    RCC_PLLCmd(ENABLE); 								 /* enalbe PLL clock */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);	 /* wait till PLL clock is ready */

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);		 /* select  PLL clock as system clock source */
    while(RCC_GetSYSCLKSource()!=0x08);				 /* wait till PLL clock is used as system clock source */

	RCC_APB2PeriphClockCmd(
		RCC_APB2Periph_GPIOA							/*两个IO口使能*/
		|RCC_APB2Periph_GPIOB
		|RCC_APB2Periph_AFIO,ENABLE);		
	RCC_APB1PeriphClockCmd(
		RCC_APB1Periph_USART2							/*UART2使能*/
		|RCC_APB1Periph_TIM3							/*TIM3 使能*/
		|RCC_APB1Periph_I2C1								/*i2c1 使能*/
		,ENABLE);		
}

#else
void RCC_Config(void)
{
  	ErrorStatus  HSEStartUpStatus; 

	RCC_DeInit(); 											/* reset rcc private set  */	
	RCC_HSEConfig(RCC_HSE_ON); 							/* enable hse clock */	
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); 				/* wait till hse clock is ready */
    RCC_HSICmd(ENABLE);
	if(HSEStartUpStatus == SUCCESS) 
	{ 
		
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); /* enable flash prefetch buffer 使能预取指缓存  */		
		FLASH_SetLatency(FLASH_Latency_2); 				 /* wait 2 flash state */		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);					 /* AHB clock = system clock */	
		RCC_PCLK2Config(RCC_HCLK_Div1); 					 /* APB2 clock = AHB clock */		
		RCC_PCLK1Config(RCC_HCLK_Div2); 					 /* APB1 clock = AHB clock/2 =36M*/
	//	RCC_USBCLKConfig (RCC_USBCLKSource_PLLCLK_1Div5 );  /*USB =PLL 时钟除以 1.5  = 48M*/
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9); /* PLL clock = 8MHz*9 = 72MHz */

		RCC_PLLCmd(ENABLE); 								 /* enalbe PLL clock */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);	 /* wait till PLL clock is ready */

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);		 /* select  PLL clock as system clock source */
		while(RCC_GetSYSCLKSource()!=0x08);				 /* wait till PLL clock is used as system clock source */
	}

	
}
#endif


/*-----------------------------------------------------------------
-------------嵌套向量中断控制器（NVIC）------------------
-------------------------------------------------------------------*/
void NVIC_Config(void)
{
	

}



void UART_Config(void)
{

}

