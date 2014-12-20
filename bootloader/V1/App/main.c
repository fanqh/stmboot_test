#include "Include.h"
#include "spi.h"
#include "DK_RFM.h"
#include "stdio.h"
#include "timer.h"

uint8 pbuf[10];
unsigned char RxBuf[RxBuf_Len];
RFM69H_DATA_Type rfm69h_data;


 void *const bootfunc[]={
	(void *)BSP_IntVectSet,		                //0
	(void *)Boot_SysReset,		                //1
	(void *)Boot_ResetUsart,		            //2    
	(void *)Boot_UsartSend,		                //3
	(void *)Boot_UsartGet,                      //4
    (void *)Boot_UsartClrBuf,	                //5
	(void *)Boot_EraseUsrFlash,	                //6
	(void *)Boot_ProgramUsrFlash,	            //7
	(void *)Boot_UpdateProgram,	                //8
	(void *)debug_led_on,                       //9
	(void *)debug_led_off,                      //10
	(void *)app_enroll_tick_hdl,                //11
	(void *)Get_ParaFrmBoot,                    //12
	(void *)boot_get_ver,                       //13
	(void *)TA_GetCommand,                      //14
	(void *)Command_Process,                    //15
	(void *)get_usart_interrupt_flg,                    //16
	0
};

__asm void exit_bios(uint32 func,uint32 addr)
{
	bx r1
}





int main(void)
{
    uint8 status;
    Boot_ResetSysClk(1,1);	
    Boot_RCC_Config();
    Boot_NVIC_Config();
    BSP_IntInit(); 
    Boot_IoInit();
    Boot_UsartInit();
    SysTick_Init(TICKS_10US);

	app_enroll_tick_hdl(isr_13us, 0);   //13us在底层配置的，配置完成就关闭了


//	SpiMsterGpioInit(SPI_2);
	TIM3_NVIC_Configuration();	
#if 1	
	SpiMsterGpioInit(SPI_2);

 	RFM69H_Config();
	RFM69H_EntryRx();
	
//		printf("system is working\r\n");	
//	
	while(1)
	{	   
	   uint8_t uu;
	   		int len1,len2;

//		SPIWrite(SPI_2, 0x0632);
//		uu = SPIRead(SPI_2, 0x06);
//		printf("%X\r\n", uu);
		 len1 = RFM69H_RxPacket(&rfm69h_data);
		 {
			Disable_SysTick();
			if(len1 > 0)
			{	
				
				printf("receive data len = %d\r\n", len1);
//				printf("receive data len = %d\r\n", len2);
#if 1
				RFM69H_EntryTx();
				if(RFM69H_TxWaitStable())
				{
					while(1)
					{
					    RFM69H_DATA_OUT = 1;
						USB_OTG_BSP_mDelay (10);
						RFM69H_SendData(&rfm69h_data);

						RFM69H_DATA_OUT = 0;
						USB_OTG_BSP_mDelay (10);
						RFM69H_SendData(&rfm69h_data);

						RFM69H_DATA_OUT = 0;
						USB_OTG_BSP_mDelay (10);
						RFM69H_SendData(&rfm69h_data);

						RFM69H_DATA_OUT = 1;
						USB_OTG_BSP_mDelay (1000);

						printf("send data len = %d\r\n", len1);
					}
				}
#endif
			  }
		 }
		 	
	}
#endif

#if 0
    Boot_UsartSend("syx",3);
    Command_Process();  
    while(1)
    {
        if(get_usart_interrupt_flg())
        {
            Boot_UsartGet(&status,1, 1);
            Boot_UsartSend(&status,1);

        }
       
    }
#endif
    if(TA_Check_Signature()==TA_SUCCESS)
    {
        exit_bios((uint32)bootfunc,APP_PROG_START+1);// FOR thumb-2 启动应用程序*/
        Command_Process();
    }
            

    return 0;    


}

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

