#ifndef _SYSTEM_VAR_H
#define _SYSTEM_VAR_H

#include "Include.h"


#define RX_DATA_BUFLEN              1024             

typedef struct
{
    volatile uint32 size;
    uint8 *write;
    uint8 *read;
}UART_INIT_STRUCT;


extern volatile uint8 db_fuf[RX_DATA_BUFLEN];
extern UART_INIT_STRUCT rx_db;

extern void (*hdl_5ms[2])(void);


#endif
