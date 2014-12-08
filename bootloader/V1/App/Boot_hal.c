#include"Boot_hal.h"

volatile uint8 sys_baud;

#define UART_RXDATA         (BOOT_UART->DR)
#define RX_BUF_START        db_fuf  
#define RX_BUF_END          (db_fuf+RX_DATA_BUFLEN)  

void Boot_RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(
		RCC_APB2Periph_GPIOA							
		|RCC_APB2Periph_GPIOB
		|RCC_APB2Periph_AFIO,ENABLE);		
	RCC_APB1PeriphClockCmd(
		RCC_APB1Periph_USART2							/*UART2使能*/
		|RCC_APB1Periph_TIM3							/*TIM3 使能*/
		,ENABLE);	

}

void Boot_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/*设置向量表的位置和偏移*/
#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 				    /*向量表位于RAM*/
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   				/*向量表位于FLASH*/
#endif	


	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				/*EXTI0_IRQn中断*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				/*优先级0*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    /*IRQ通道使能*/
	NVIC_Init(&NVIC_InitStructure);								    /*初始化外设NVIC寄存器USART2*/

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			    /*USART2中断*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				/*优先级1*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    /*IRQ通道使能*/
	NVIC_Init(&NVIC_InitStructure);	

}
void Boot_SysReset(void)
{
        NVIC_SystemReset();
}



uint32  Boot_Status(void)
{

}

uint8   get_usart_interrupt_flg(void)
{
    return rx_db.size;
}

void BOOT_UART_IRQHandler(void)
{
    __disable_irq();
	if(USART_GetITStatus(BOOT_UART, USART_IT_RXNE) != RESET)
	{	
       if(rx_db.size<RX_DATA_BUFLEN) //缓冲区未满 
       {
            if(rx_db.write>=RX_BUF_END)
        	{
        		rx_db.write =RX_BUF_START;
        	}
            *rx_db.write++=UART_RXDATA;
            rx_db.size+=1;
       }
    }
    __enable_irq();
   
}


const uint32 BaudRate[6]={0,9600,19200,38400,57600,115200};

void    Boot_ResetUsart(uint32 newbaud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    
	USART_InitStructure.USART_BaudRate = BaudRate[newbaud];
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	GPIO_InitStructure.GPIO_Pin = BOOT_UART_TX;		            //设置TX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		        //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BOOT_UART_GPIO, &GPIO_InitStructure);
	
	/* Configure USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin =BOOT_UART_RX;		            //设置RX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_Init(BOOT_UART_GPIO, &GPIO_InitStructure);
	
	/* USART configuration */
	USART_Init(BOOT_UART, &USART_InitStructure);		        //初始化USART
	
	/* Enable USART */
	USART_Cmd(BOOT_UART, ENABLE);		                       
	
	USART_ITConfig(BOOT_UART, USART_IT_RXNE, ENABLE);           //接受中断
	while (USART_GetFlagStatus(BOOT_UART, USART_FLAG_TC) == RESET);
	BSP_IntVectSet(INT_USART_ID,BOOT_UART_IRQHandler);  

}
void    Boot_UsartInit(void)
{
    Boot_UsartClrBuf();
    sys_baud=TA_BAUD_115200;
    Boot_ResetUsart(sys_baud);
   // Reset_UsartDMA(cmd_data,0);

}
void    Boot_UsartSend(unsigned char *outptr,unsigned int len)
{
    while(len--)
	{
		USART_SendData(BOOT_UART,(unsigned char)*outptr++);
		while (USART_GetFlagStatus(BOOT_UART, USART_FLAG_TC) == RESET);
    }
	

}



int16   Boot_UsartGet(uint8 *buff,uint32 len,uint32 timeout)
{
    uint32 i,tmpLen;    

    if(rx_db.size>=len)		            //数据可用
	{
         __disable_irq();
        if(rx_db.read+len>RX_BUF_END)  //>=
        {
           
            tmpLen =RX_BUF_END-rx_db.read;
            memcpy(buff,rx_db.read,tmpLen);
            rx_db.read=RX_BUF_START;
            memcpy(buff+tmpLen,(uint8 *)rx_db.read,len-tmpLen);
            rx_db.read+=len-tmpLen;
            rx_db.size-=len;
        }
        else
        {
            memcpy(buff,rx_db.read,len);
            rx_db.read+=len;
            rx_db.size-=len;
        }
        __enable_irq();
        return len;
	}
    i=0;
    while(1)
    {
        delay_us(1000);
        i++;
        if(rx_db.size>=len)		            //数据可用
    	{
             __disable_irq();
    		if(rx_db.read+len>RX_BUF_END)   //>==
            {
                tmpLen =RX_BUF_END-rx_db.read;
                memcpy(buff,rx_db.read,tmpLen);
                rx_db.read=RX_BUF_START;
                memcpy(buff+tmpLen,(uint8 *)rx_db.read,len-tmpLen);
                rx_db.read+=len-tmpLen;
                rx_db.size-=len;
            }
            else
            {
                memcpy(buff,(uint8 *)rx_db.read,len);
                rx_db.read+=len;
                rx_db.size-=len;
            }
            __enable_irq();
            return len;
    	}
        else
        {
            if(i>=timeout && timeout)
            {
                __disable_irq();    
                if(rx_db.read+rx_db.size>=RX_BUF_END)
                {
                    tmpLen =RX_BUF_END-rx_db.read;
                    memcpy(buff,(uint8 *)rx_db.read,tmpLen);
                    rx_db.read=RX_BUF_START;
                    memcpy(buff+tmpLen,(uint8 *)rx_db.read,rx_db.size-tmpLen);
                    rx_db.read+=rx_db.size-tmpLen;                   
                    tmpLen=rx_db.size;
                    rx_db.size=0;
                }
                else
                {
            		memcpy(buff,(uint8 *)rx_db.read,rx_db.size);
                    rx_db.read+=rx_db.size;
                    tmpLen=rx_db.size;
                    rx_db.size=0;
                }
                __enable_irq();
                return tmpLen;
            }
        }
        

    }

}

void    Boot_UsartClrBuf(void)
{
    __disable_irq();
    rx_db.size=0;
    rx_db.write=RX_BUF_START;
    rx_db.read=RX_BUF_START;
    memset(db_fuf,0x00,sizeof(db_fuf));
    __enable_irq();
}

void    Boot_IoInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 /*---推挽输出------*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = DEBUG_LED_PIN; 
	GPIO_Init(BEBUG_LED_GPIO,&GPIO_InitStructure);					/*----led 引脚--------*/

}

void debug_led_on(void)
{
    GPIO_SetBits(BEBUG_LED_GPIO,DEBUG_LED_PIN);
}

void debug_led_off(void)
{
    GPIO_ResetBits(BEBUG_LED_GPIO,DEBUG_LED_PIN);
}
uint32  Boot_UpdateIoStatus(void)
{


}

void    Boot_EnterLPMode(void)
{


}

uint32 Boot_GetSector(uint32 sector)
{	
     
}

int32   Boot_EraseFlashSector(uint32 sector)
{
        uint32 i=0,result;
        if(sector>7 ||sector<2)
            return TA_FAIL;
        FLASH_Unlock();
        FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//清标志位 
        for(i=0;i<SECTOR_LEN/1024;i++)
        {
            while(FLASH_COMPLETE!=FLASH_ErasePage(BOOTLOADER_START+sector*SECTOR_LEN+i*1024));
        }
        FLASH_Lock();
        return TA_SUCCESS;

}
int32   Boot_EraseUsrFlash(uint32 addr)
{


}

int32   Boot_ProgramFlash(uint8 *buff,uint32 addr,uint32 len)
{
	uint32 data;
	uint32 i;
	uint16  *p8_read,tmp;
	uint32 *p;
	p =(uint32 *)addr;
	FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//清标志位
	while(len>=4)
	{
		data=0;
		for(i=0;i<4;i++)
		{
			data>>=8;
			data|=(*buff++)<<24;
		}
        //__disable_irq();
		FLASH_ProgramWord(addr,data);
        //__enable_irq();
		if(*p++!=data)
			return TA_FLASH_ERR;
		addr+=4;
		len-=4;
	}
    for(i=0;i<len;i++)
	{
		tmp>>=8;
		tmp|=(*buff++)<<8;
	}
    //__disable_irq();
	FLASH_ProgramHalfWord(addr,tmp);
    // __enable_irq();
	FLASH_Lock();

	return TA_SUCCESS;	

}

int32   Boot_ProgramUsrFlash(uint8 *buff,uint32 addr,uint32 len)
{


}

void    Boot_ResetSysClk(uint32 newsysclk,uint32 source)
{
 
	RCC_DeInit(); 											/* reset rcc private set  */
    #if 0
	RCC_HSEConfig(RCC_HSE_ON); 							    /* enable hse clock */	
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); 			/* wait till hse clock is ready */
    RCC_HSICmd(ENABLE);
	if(HSEStartUpStatus == SUCCESS) 
	{ 
		
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); /* enable flash prefetch buffer 使能预取指缓存  */		
		FLASH_SetLatency(FLASH_Latency_2); 				     /* wait 2 flash state */		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);					 /* AHB clock = system clock */	
		RCC_PCLK2Config(RCC_HCLK_Div1); 					 /* APB2 clock = AHB clock */		
		RCC_PCLK1Config(RCC_HCLK_Div2); 					 /* APB1 clock = AHB clock/2 =36M*/

        RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9); /* PLL clock = 8MHz*9 = 72MHz */

		RCC_PLLCmd(ENABLE); 								 /* enalbe PLL clock */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);	 /* wait till PLL clock is ready */

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);		 /* select  PLL clock as system clock source */
		while(RCC_GetSYSCLKSource()!=0x08);				 /* wait till PLL clock is used as system clock source */
	}
    #else
    RCC_AdjustHSICalibrationValue(0x10); 
    RCC_HSICmd(ENABLE);
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); /* enable flash prefetch buffer 使能预取指缓存  */		
    FLASH_SetLatency(FLASH_Latency_2); 				     /* wait 2 flash state */		
    RCC_HCLKConfig(RCC_SYSCLK_Div1);					 /* AHB clock = system clock */	
    RCC_PCLK2Config(RCC_HCLK_Div1); 					 /* APB2 clock = AHB clock */		
    RCC_PCLK1Config(RCC_HCLK_Div2); 					 /* APB1 clock = AHB clock/2 =36M*/

    RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16); /* PLL clock = 4MHz*16 = 64MHz */

    RCC_PLLCmd(ENABLE); 								 /* enalbe PLL clock */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);	 /* wait till PLL clock is ready */

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);		 /* select  PLL clock as system clock source */
    while(RCC_GetSYSCLKSource()!=0x08);				 /* wait till PLL clock is used as system clock source */
    #endif

}

void    Flash_Code_Protect(void)
{
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//清标志位
    FLASH_EnableWriteProtection(FLASH_WRProt_Pages0to3); 
    FLASH_EnableWriteProtection(FLASH_WRProt_Pages4to7); 
    FLASH_EnableWriteProtection(FLASH_WRProt_Pages8to11); 
    FLASH_EnableWriteProtection(FLASH_WRProt_Pages12to15);     
    FLASH_Lock();

}

void    Flash_Code_UnProtect(void)
{


}



void delay_us(uint32 times)
{
    int i;
    while(times--)
    {
       
        i=3;
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        while(i--)
        {
           __NOP();
           __NOP();
           __NOP();
           __NOP();
           __NOP();
           
        }
    }

}


