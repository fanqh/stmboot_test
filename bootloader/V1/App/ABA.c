#include"ABA.h"

const uint16 uwMatchPara[3][8]={
{0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff},
{0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff}
};

void Get_Key(uint8 key[])
{
	uint32 *pid,data,id[3];
	data=0xE00085EF;								
	pid=(uint32 *)(data^(~0));						
	id[0]=*pid++;
	id[1]=*pid++;
	id[2]=*pid++;
	id[0]^=id[2]^0x12345678;
	id[1]+=id[2];
	id[1]+=0x8668;	
	// 根据M3自带ID产生看起来唯一的64bit加密key
	memcpy(key,(uint8 *)id,8);
}
const uint8 lcl[8]={0xad,0xfd,0x4f,0x89,0x45,0x67,0x56,0x12};//密钥
const uint8 user_data[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
const uint8 syx[8]={0x24,0x30,0x31,0x2D,0x2A,0x37,0x7F,0x36};	//密钥
uint8 en_data[8];
void Get_ParaFrmBoot(uint16 iLevel,uint16 version,uint8 *addr)
{
	uint8 key[8],*para;
	
	Get_Key(key);
	if(version==54321)
	{
		undes((uint8 *)lcl,(uint8 *)user_data,addr);	//addr 得到的id值
		memcpy(addr+8,key,8);
	}	
}


void Init_MatchPara(void)
{
#define enc(x)		((x*5)^0x1234)
#define denc(x)		((x^0x1234)/5)
	uint32 	i,total;
	uint16 	*pdata;
	uint16 	uwConf[3][8]={
		{enc(650),enc(95),enc(285),enc(5000),enc(4800),enc(3800),enc(1950),enc(9950)},
		{enc(580),enc(93),enc(285),enc(5200),enc(5000),enc(4650),enc(1950),enc(9950)},
		{enc(580),enc(93),enc(285),enc(5400),enc(5200),enc(4950),enc(1950),enc(9950)}
	};	// 确保在FLASH中看不到正确的参数
	uint8 	paradata[sizeof(uwConf)],key[8],*paramsrc,*paramdest;
	
	total=0;
	pdata=(uint16 *)(uwMatchPara[0]);
	for(i=0;i<sizeof(uwMatchPara)/2;i++)
		total+=*pdata++;
	
	if(total==0xffff*sizeof(uwMatchPara)/2)
		{	// 仅第一次初始化的时候加密参数到存储区uwMatchPara
		
		pdata=uwConf[0];
		for(i=0;i<sizeof(uwConf)/2;i++)
			{
			*pdata=denc(*pdata);
			pdata++;
		}
		
		Get_Key(key);

		paramsrc=(uint8 *)(uwConf[0]);
		paramdest=paradata;
		for(i=0;i<sizeof(uwConf)/8;i++)
			{
			des(key,paramsrc,paramdest);
			paramsrc+=8;
			paramdest+=8;
		}
		Boot_ProgramFlash(paradata,(uint32)uwMatchPara,sizeof(paradata));
	}	
}


void Api_Init(void)
{
	uint32 	i,total;
	uint8	*pdata;
	uint8  srcid[8];
	total=0;
	pdata=(uint8 *)user_data;
	for(i=0;i<sizeof(user_data);i++)
		total+=*pdata++;
	
	if(total==0xff*sizeof(user_data))// 仅第一次初始化的时候加密参数到存储区uwMatchPara
	{	
		memset(srcid,0x00,sizeof(srcid));
		Get_Key(srcid);
		des((uint8*)lcl, srcid,en_data);	
		Boot_ProgramFlash(en_data,(uint32)user_data,sizeof(en_data));
	}	


}







void Get_id( )
{

	uint8 Srcid[8];
	memset(Srcid,0x00,sizeof(Srcid));
	Get_Key(Srcid);
//	Boot_UsartSend(Srcid, 8);
	des((uint8*)syx, Srcid,en_data);
}


void LXSQ(void)
{
	uint32 	i;
	uint16  check;	
	uint16 	buf[24+1],*pdata;
	uint8 	xor;
	TA_AckCommand(CMD_AUTHORIZE, 0, 0, 9);

	Get_id();
	xor =0;
	Boot_UsartSend(en_data, 8);
	for(i=0;i<8;i++)
		xor^=en_data[i];
	Boot_UsartSend(&xor,1);

	Boot_UsartGet((uint8 *)buf, 1, 0);	//去掉0xf5
	memset(buf,0x00,sizeof(buf));
	Boot_UsartGet((uint8 *)buf, sizeof(buf), 0);
	check =0;
	for(i=0;i<24;i++)
		check^=buf[i];
	if(buf[24]==check)
	{
		
		Boot_ProgramFlash((uint8 *)buf,(uint32)uwMatchPara,sizeof(uwMatchPara));
		pdata =(uint16 *)&uwMatchPara[0][0];
		for(i=0;i<sizeof(uwMatchPara)/2;i++)
			if(buf[i]!=*pdata++)
				break;
		if(i==sizeof(uwMatchPara)/2)	
			TA_AckCommand(CMD_AUTHORIZE, 0, 0, 0);	//获取数据成功
		else
			TA_AckCommand(CMD_AUTHORIZE, 0, 0, TA_FLASH_ERR);		
	}
	else
		TA_AckCommand(CMD_AUTHORIZE, 0, 0, TA_CHECK_ERROR);	//获取数据成功

}

uint8 LXSQ_Judge(void)
{
	uint32 	i,total;
	uint16 	*pdata;
	total=0;
	pdata=(uint16 *)(uwMatchPara[0]);
	for(i=0;i<sizeof(uwMatchPara)/2;i++)
		total+=*pdata++;
	
	if(total==0xffff*sizeof(uwMatchPara)/2)
		return  TA_FAIL;
	else
		return  TA_SUCCESS;	



}

