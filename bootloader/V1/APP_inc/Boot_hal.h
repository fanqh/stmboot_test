#ifndef _BOOT_HAL_H
#define _BOOT_HAL_H

#include "Include.h"


#define BOOT_UART			    USART2
#define BOOT_UART_TX		    GPIO_Pin_2
#define BOOT_UART_RX		    GPIO_Pin_3
#define BOOT_UART_GPIO		    GPIOA
#define INT_USART_ID		    BSP_INT_ID_USART2

#define DEBUG_LED_PIN			GPIO_Pin_4
#define BEBUG_LED_GPIO          GPIOA


void    Boot_RCC_Config(void);

void    Boot_NVIC_Config(void);

void    Boot_SysReset(void);

uint32  Boot_Status(void);

void    Reset_UsartDMA(uint8 *data,uint16 buff_len);

void    Set_UsartIrq(uint32 status);

void    Boot_ResetUsart(uint32 newbaud);

void    Boot_UsartInit(void);

void    Boot_UsartSend(unsigned char *outptr,unsigned int len);

int16   Boot_UsartGet(uint8 *buff,uint32 len,uint32 timeout);

void    Boot_UsartClrBuf(void);

void    Boot_IoInit(void);

void    debug_led_on(void);

void    debug_led_off(void);

uint32  Boot_UpdateIoStatus(void);

void    Boot_EnterLPMode(void);

int32   Boot_EraseFlashSector(uint32 sector);

int32   Boot_EraseUsrFlash(uint32 addr);

int32   Boot_ProgramFlash(uint8 *buff,uint32 addr,uint32 len);

int32   Boot_ProgramUsrFlash(uint8 *buff,uint32 addr,uint32 len);

void    Boot_ResetSysClk(uint32 newsysclk,uint32 source);

void    Flash_Code_Protect(void);

void    Flash_Code_UnProtect(void);

uint8   get_usart_interrupt_flg(void);

void    delay_us(uint32 times);





#endif
