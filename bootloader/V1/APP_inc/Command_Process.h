#ifndef  _COMMAND_PROCESS_H
#define  _COMMAND_PROCESS_H


#include"Include.h"






uint32  TA_Check_Signature(void);

void    boot_get_ver(unsigned char *buf);

void    TA_GetBootVer(void);

void    Boot_UpdateProgram(uint32 binLen);

void    TA_GetCommand(unsigned char *cmd);

void    TA_AckCommand(uint8 cmd,uint8 var1,uint8 var2,uint8 result);

void    Command_Process(void);

#endif

