#include"Timer.h"
#include "DK_RFM.h"


#if 0
extern  volatile unsigned int T ;	//����
extern  uint32  F;     //�Ƿ��38KH��������
extern  uint32  Wifi_AP_OPEN_MODE;     //�Ƿ��38KH��������
extern  uint32  Wifi_MAC_Count;
extern  uint32  Get_Wifi_MAC;
volatile uint32 Turn=1;             //��ת��־
volatile uint8  read_led_status=0;

uint16_t ted = 0;
extern unsigned char RxBuf[RxBuf_Len];


//5ms
void TIMER2_Handler(void)
{
	unsigned char Rec;
  
    TIM2->SR = ~TIM_FLAG_Update;  //�����־λ
    #if 1
    if(Wifi_AP_OPEN_MODE)
	{
		Wifi_AP_OPEN_MODE++;
		if(Wifi_AP_OPEN_MODE == 50)
		{
            read_led_status =~read_led_status;    
			wifi_led(read_led_status);
			Wifi_AP_OPEN_MODE = 1;
		}
	}
	else
	{
		Wifi_MAC_Count++;
		ted ++;
     
		if(Wifi_MAC_Count == 2*200) //3�붨ʱ
		{
			Get_Wifi_MAC = 1;
			Wifi_MAC_Count = 0;
		}
		if(ted == 200)	
		{
			uint8 i = 0;

			ted = 0;
			Rec = RFM69H_RxPacket(RxBuf);
			if(Rec == 1)  //��������
		  	{
				Rec = 0;
				//WAKEUP_LED = ~WAKEUP_LED;
				if(RxBuf[0] != RxBuf[1])
				{
					U1_send(0x7E);
					U1_send(0x7F);
					for(i=0;i<RxBuf_Len;i++)	
					{
						U1_send(RxBuf[i]);//Uart_sendB(RxBuf[i]);
					}
					U1_sendS("<<",2);
		 			RFM69H_EntryRx();
				}
		  	}
			else
			{
				RFM69H_EntryRx();
			}
		}
	}
    #else
    read_led_status =~read_led_status;    
    wifi_led(read_led_status);
    #endif
}




/* TIM2�ж����ȼ����� */
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*�ж�����Ϊ5ms*/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=5000;	//�Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (64 - 1);	//ʱ��Ԥ��Ƶ�� 72M/72      
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// �������жϱ�־ 
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);	// ����ʱ��    
    BSP_IntVectSet(BSP_INT_ID_TIM2, TIMER2_Handler);
 }

void timer2_init(void)
{
     TIM2_NVIC_Configuration();
     TIM2_Configuration();
  //   timer2_disable();

}
void timer2_enable(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);	
}
void timer2_disable(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);
}


#endif



void isr_13us(void)
{
//    T++;
//    if(F)                                                          
//	{
//        if (Turn)
//        {
//            Turn=0;
//            SET_INFRARED;
//        }
//        else
//        {
//            Turn=1;
//            CLR_INFRARED;
//        }
//    }
	if(Get_RFM69H_Status()!=RFM69H_IDLE)
	{
		DataTimeCount ++;
	}

}

//void Disable_SysTick(void)
//{
//     SysTick->CTRL &= (~(1<<SYSTICK_ENABLE) ); 
//}
//void Enable_SysTick(void)
//{
//    SysTick->CTRL |= ((1<<SYSTICK_ENABLE) ); 
//}


#define TIM_MSEC_DELAY                     0x01
#define TIM_USEC_DELAY                     0x02

volatile  uint32 BSP_delay = 0;



void USB_OTG_BSP_TimerIRQ (void)
{
    
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    if (BSP_delay > 0x00)
    { 
      BSP_delay--;
    }
    else
    {
      TIM_Cmd(TIM3,DISABLE);
    }
  }
} 

/* TIM3�ж����ȼ����� M3*/
void TIM3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	BSP_IntVectSet(BSP_INT_ID_TIM3, USB_OTG_BSP_TimerIRQ);
}



/**
  * @brief  BSP_SetTime
  *         Configures TIM2 for delay routine based on TIM2
  * @param  unit : msec /usec
  * @retval None
  */
static void BSP_SetTime(uint8_t unit)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  TIM_Cmd(TIM3,DISABLE);
  TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE); 
  
  
  if(unit == TIM_USEC_DELAY)
  {  
    TIM_TimeBaseStructure.TIM_Period = 1;		 //1us
  }
  else if(unit == TIM_MSEC_DELAY)
  {
    TIM_TimeBaseStructure.TIM_Period = 1000-1;	  //ms
  }
  TIM_TimeBaseStructure.TIM_Prescaler = 64-1;  //1Mʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  
  TIM_ARRPreloadConfig(TIM3, ENABLE);
  
  /* TIM IT enable */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  
  /* TIM2 enable counter */ 
  TIM_Cmd(TIM3, ENABLE);  
} 




static void BSP_Delay(uint32_t nTime, uint8_t unit)
{
  
  BSP_delay = nTime;
  BSP_SetTime(unit);  
  while(BSP_delay != 0);
  TIM_Cmd(TIM3,DISABLE);
}



void USB_OTG_BSP_uDelay (const uint32_t usec)
{
 
  BSP_Delay(usec,TIM_USEC_DELAY); 
 
}


/**
  * @brief  USB_OTG_BSP_mDelay
  *          This function provides delay time in milli sec
  * @param  msec : Value of delay required in milli sec
  * @retval None
  */
void USB_OTG_BSP_mDelay (const uint32_t msec)
{  
    BSP_Delay(msec,TIM_MSEC_DELAY);   

}



