#include "Include.h"
#include "spi.h"
#include "DK_RFM.h"
#include "stdio.h"
#include "timer.h"

uint8 pbuf[10];


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
    SysTick_Init(TICKS_13US);

	app_enroll_tick_hdl(isr_13us, 0);   //13us在底层配置的，配置完成就关闭了
	

	
	SpiMsterGpioInit(SPI_2);

 	RFM69H_Config();
	RFM69H_EntryRx();
	
//		printf("system is working\r\n");	
//	
	while(1)
	{	   
		int len =0;
//		printf("system is working\r\n");
//		 RFM69H_EntryRx();
		 if(RFM69H_RxPacket(pbuf))
		 {	
		 	len = RFM69H_Analysis();
			Disable_SysTick();
			if(len > 0)
			{	
				RFM69H_EntryTx();
				if(RFM69H_TxWaitStable())
				{
					while(1)
					{
						RFM69H_SendData(&rfm69h_data);
						printf("data len = %d\r\n", len);
					}
				}
			}
		 }
		 	
	}


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

