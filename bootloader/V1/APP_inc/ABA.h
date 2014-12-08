#ifndef  _ABA_H
#define _ABA_H

#include "Include.h"






extern  const uint16 uwMatchPara[3][8];
void    Get_Key(uint8 key[]);
void    Get_ParaFrmBoot(uint16 iLevel,uint16 version,uint8 *addr);


int     Get_Data(uint32 len);
void    des_calc(void);
void    Init_MatchPara(void);

uint8   LXSQ_Judge(void);


#endif
