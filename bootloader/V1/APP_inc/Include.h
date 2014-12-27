#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__


#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  "math.h"



typedef char				int8  ;						// 8 bits
typedef short				int16 ;						// 16 bits
typedef int				    int32 ;						// 32 bit integer

typedef unsigned char		uint8  ;					// unsigned 8 bits
typedef unsigned short		uint16 ;					// unsigned 16 bits
typedef unsigned int		uint32 ;					// unsigned 32 bit integer
typedef uint8               BOOL;


#include "Stm32f103_it.h"

#include  <stm32f10x_conf.h>
#include    "cpu.h"








#include"System_var.h"

#include"config.h"
#include "SysTick.h"
#include "App_define.h"

#include "System_var.h"
#include "Boot_hal.h"

#include "des.h"
#include "ABA.h"
#include"Command_Process.h"


#define TIME_UNIT		10		//系统定时器时间


#endif

