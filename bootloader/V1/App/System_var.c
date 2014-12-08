#include"System_var.h"
#include"App_define.h"

volatile uint8 db_fuf[RX_DATA_BUFLEN];

UART_INIT_STRUCT rx_db;

void (*hdl_5ms[2])(void);

