#include"Stm32f103_it.h"
#include"System_var.h"


static  CPU_FNCT_VOID  BSP_IntVectTbl[BSP_INT_SRC_NBR];



static  void  BSP_IntHandler     (CPU_DATA  int_id);

void  BSP_IntInit (void)
{
    CPU_DATA  int_id;


    for (int_id = 0; int_id < BSP_INT_SRC_NBR; int_id++) {
        BSP_IntVectSet(int_id, BSP_IntHandlerDummy);
    }
}
void BSP_IntHandlerDummy(void)
{
}

void  OS_CPU_SysTickHandler(void)
{
    if(hdl_5ms[0]) 
		hdl_5ms[0]();
	if(hdl_5ms[1]) 
		hdl_5ms[1]();
    
}
void OS_CPU_PendSVHandler(void)
{
}

void  BSP_IntHandlerWWDG(void)  { BSP_IntHandler(BSP_INT_ID_WWDG);            }
void  BSP_IntHandlerPVD (void)  { BSP_IntHandler(BSP_INT_ID_PVD);             }
void  BSP_IntHandlerTAMPER  (void)  { BSP_IntHandler(BSP_INT_ID_TAMPER);          }
void  BSP_IntHandlerRTC(void)  { BSP_IntHandler(BSP_INT_ID_RTC);             }
void  BSP_IntHandlerFLASH(void)  { BSP_IntHandler(BSP_INT_ID_FLASH);           }
void  BSP_IntHandlerRCC  (void)  { BSP_IntHandler(BSP_INT_ID_RCC);             }
void  BSP_IntHandlerEXTI0(void)  { BSP_IntHandler(BSP_INT_ID_EXTI0);           }
void  BSP_IntHandlerEXTI1(void)  { BSP_IntHandler(BSP_INT_ID_EXTI1);           }
void  BSP_IntHandlerEXTI2  (void)  { BSP_IntHandler(BSP_INT_ID_EXTI2);           }
void  BSP_IntHandlerEXTI3 (void)  { BSP_IntHandler(BSP_INT_ID_EXTI3);           }
void  BSP_IntHandlerEXTI4 (void)  { BSP_IntHandler(BSP_INT_ID_EXTI4);           }
void  BSP_IntHandlerDMA1_CH1(void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH1);        }
void  BSP_IntHandlerDMA1_CH2(void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH2);        }
void  BSP_IntHandlerDMA1_CH3(void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH3);        }
void  BSP_IntHandlerDMA1_CH4(void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH4);        }
void  BSP_IntHandlerDMA1_CH5(void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH5);        }
void  BSP_IntHandlerDMA1_CH6(void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH6);        }
void  BSP_IntHandlerDMA1_CH7(void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH7);        }
void  BSP_IntHandlerADC1_2  (void)  { BSP_IntHandler(BSP_INT_ID_ADC1_2);          }
void  BSP_IntHandlerUSB_HP_CAN_TX (void)  { BSP_IntHandler(BSP_INT_ID_USB_HP_CAN_TX);   }
void  BSP_IntHandlerUSB_LP_CAN_RX0(void)  { BSP_IntHandler(BSP_INT_ID_USB_LP_CAN_RX0);  }
void  BSP_IntHandlerCAN_RX1(void)  { BSP_IntHandler(BSP_INT_ID_CAN_RX1);         }
void  BSP_IntHandlerCAN_SCE(void)  { BSP_IntHandler(BSP_INT_ID_CAN_SCE);         }
void  BSP_IntHandlerEXTI9_5(void)  { BSP_IntHandler(BSP_INT_ID_EXTI9_5);         }
void  BSP_IntHandlerTIM1_BRK(void)  { BSP_IntHandler(BSP_INT_ID_TIM1_BRK);        }
void  BSP_IntHandlerTIM1_UP(void)  { BSP_IntHandler(BSP_INT_ID_TIM1_UP);         }
void  BSP_IntHandlerTIM1_TRG_COM(void)  { BSP_IntHandler(BSP_INT_ID_TIM1_TRG_COM);    }
void  BSP_IntHandlerTIM1_CC (void)  { BSP_IntHandler(BSP_INT_ID_TIM1_CC);         }
void  BSP_IntHandlerTIM2(void)  { BSP_IntHandler(BSP_INT_ID_TIM2);            }
void  BSP_IntHandlerTIM3 (void)  { BSP_IntHandler(BSP_INT_ID_TIM3);            }
void  BSP_IntHandlerTIM4(void)  { BSP_IntHandler(BSP_INT_ID_TIM4);            }
void  BSP_IntHandlerI2C1_EV(void)  { BSP_IntHandler(BSP_INT_ID_I2C1_EV);         }
void  BSP_IntHandlerI2C1_ER(void)  { BSP_IntHandler(BSP_INT_ID_I2C1_ER);         }
void  BSP_IntHandlerI2C2_EV(void)  { BSP_IntHandler(BSP_INT_ID_I2C2_EV);         }
void  BSP_IntHandlerI2C2_ER(void)  { BSP_IntHandler(BSP_INT_ID_I2C2_ER);         }
void  BSP_IntHandlerSPI1(void)  { BSP_IntHandler(BSP_INT_ID_SPI1);            }
void  BSP_IntHandlerSPI2(void)  { BSP_IntHandler(BSP_INT_ID_SPI2);            }
void  BSP_IntHandlerUSART1(void)  { BSP_IntHandler(BSP_INT_ID_USART1);          }
void  BSP_IntHandlerUSART2(void)  { BSP_IntHandler(BSP_INT_ID_USART2);          }
void  BSP_IntHandlerUSART3(void)  { BSP_IntHandler(BSP_INT_ID_USART3);          }
void  BSP_IntHandlerEXTI15_10(void)  { BSP_IntHandler(BSP_INT_ID_EXTI15_10);       }
void  BSP_IntHandlerRTCAlarm(void)  { BSP_IntHandler(BSP_INT_ID_RTCAlarm);        }
void  BSP_IntHandlerUSBWakeUp(void){ BSP_IntHandler(BSP_INT_ID_USBWakeUp);       }



void  BSP_IntVectSet (CPU_DATA       int_id,      CPU_FNCT_VOID  isr)
{
   	 CPU_SR   cpu_sr;
  	 if (int_id < BSP_INT_SRC_NBR) 
	{
	        CPU_CRITICAL_ENTER();
	        BSP_IntVectTbl[int_id] = isr;
	        CPU_CRITICAL_EXIT();
    	}
}



static  void  BSP_IntHandler (CPU_DATA  int_id)
{
	CPU_FNCT_VOID  isr;
	if (int_id < BSP_INT_SRC_NBR) 
	{
	        	isr = BSP_IntVectTbl[int_id];
	        if (isr != (CPU_FNCT_VOID)0) 
		{
	           	 isr();
	        }
	}

}





