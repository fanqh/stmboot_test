#ifndef _CONFGI_H_
#define _CONFGI_H_


#include "Include.h"


#define DATA_UART_TX				GPIO_Pin_2
#define DATA_UART_RX				GPIO_Pin_3




enum led_status{OFF=0,ON=!OFF};



void RCC_Config(void);
void NVIC_Config(void);
void UART_Config(void);








#endif
