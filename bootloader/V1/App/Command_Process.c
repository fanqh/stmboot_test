#include"Command_Process.h"


void Command_Process(void)
{
    uint8 pcmd[8];
    uint32 len;
    while(1)
    {

        TA_GetCommand(pcmd);
        switch(pcmd[1])
        {
            case CMD_GET_VERSION:	
                TA_GetBootVer();           break;

            case CMD_PROG_UPGRADE:
                len=((uint32)pcmd[2]<<24)+((uint32)pcmd[3]<<16)+((uint32)pcmd[4]<<8)+((uint32)pcmd[5]);
                Boot_UpdateProgram(len);
                break;
            case CMD_AUTHORIZE:
                LXSQ();
                Boot_SysReset();				/*ϵͳ��λ*/
                break;
            default:
                TA_AckCommand(pcmd[1],0,0,TA_FAIL);
            break;        
        }
              
    }



}

uint32 TA_Check_Signature(void)
{
    char *pver;
    pver=(char *)(APP_PROG_START);
    if(pver[4]=='T'&&pver[5]=='A'&&pver[6]=='0'&&pver[7]=='1')
            return TA_SUCCESS;
    else 
            return TA_FAIL;
}



const char IDBuf[]="bootloader:M3_131208_V1";
const char VerEnd[]="M.S Co.ltd. All rights reserved.";


void   boot_get_ver(unsigned char *buf)
{
    memcpy(buf,&IDBuf[11],12);
}



void TA_GetBootVer(void)
{
        unsigned char i,Xor,head;
        char sendbuf[120];
        char *pver;
        char *sendp;
        Xor=0;
        head=0xF5;
        memset(sendbuf,0x00,120);
        sendp=sendbuf;
        *sendp++=head;
        strcpy(sendp,IDBuf);
        strcat(sendp,"\x0d\x0a");
        pver=(char *)(APP_PROG_START);
        if(pver[4]=='T'&&pver[5]=='A'&&pver[6]=='0'&&pver[7]=='1')
                {
        	strcat(sendp,pver+12);
        	strcat(sendp,":");
        	strcat(sendp,pver+29);
        }
        else strcat(sendp,"NO app.");
        strcat(sendp,"\x0d\x0a");
        strcat(sendp,VerEnd);
        for(i=0;i<BOOT_VERSIONSIZE;i++)
        	Xor^=*sendp++;
        *sendp++=Xor;
        *sendp++=head;
        TA_AckCommand(CMD_GET_VERSION,BOOT_VERSIONSIZE>>8,BOOT_VERSIONSIZE&0xff,TA_SUCCESS); 
        Boot_UsartSend((unsigned char *)sendbuf,sendp-sendbuf);
}


uint8 updatabin[10*1024];

void    Boot_UpdateProgram(uint32  Binlen)
{
	uint8 databuf[PKG_LEN];
	int32 len,result,bakeup_Len;
	uint32 i,addr,Count;
	uint8 packlen,xor;
    uint8 *pUpg;
	bakeup_Len =len=Binlen;
	addr=0;
    #if  0
    Boot_EraseFlashSector(6);
	Boot_EraseFlashSector(7);       //������������
	#else
    pUpg=updatabin;
    #endif
    Disable_SysTick();
	TA_AckCommand(CMD_PROG_UPGRADE,0,0,TA_SUCCESS);	//
	
    packlen=Boot_UsartGet(databuf,PKG_LEN,0);

	if(packlen!=PKG_LEN)
	{
		TA_AckCommand(CMD_PROG_UPGRADE,0,0,TA_PKGR_LEN_ERR);return;	
	}
	Count=1;
	xor=databuf[packlen-1];
	for(i=0;i<packlen-1;i++)
		xor^=databuf[i];
	if(xor)
	{
		TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_CHECK_ERROR);	//��һ������У�����
		return;
	}		
	TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_SUCCESS);//��һ������У��ɹ�
	if(databuf[4]=='T'&&databuf[5]=='A'&&databuf[6]=='0'&&databuf[7]=='1')			//����ǲ��Ǳ���˾�ĳ���
	{
		addr=(uint32)databuf[11]<<24|(uint32)databuf[10]<<16|(uint32)databuf[9]<<8|databuf[8];
		if(addr==APP_PROG_START)
		{
			if(len>APP_PROG_LEN)
			{
				TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_LEN_OVERFLOW);
				return;
			}
		}
		else
		{   // ��д��ַ����ż����ַ
			TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_ADDR_FORMAT_ERR);//��ַ��ʽ����
			return;
		}
	}
	else
	{
		TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_PRO_FORMAT_ERR);	//�����ʽ����
		return;
	}
    #if 0
//	Boot_EraseFlashSector(6);
//	Boot_EraseFlashSector(7);
	result=Boot_ProgramFlash(databuf,DB_START_ADDR+(Count-1)*REAL_PRO_PKG_LEN,REAL_PRO_PKG_LEN);
    #else
    memcpy(pUpg+(Count-1)*REAL_PRO_PKG_LEN,databuf,REAL_PRO_PKG_LEN);
    result=TA_SUCCESS;
    #endif
	len-=REAL_PRO_PKG_LEN;
	if(result!=TA_SUCCESS)
	{
		TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_FLASH_ERR);	//��Count������У�����
		return;
	}
	while(1)
	{
		packlen=Boot_UsartGet(databuf,PKG_LEN,0);
		Count++;
		if(packlen!=PKG_LEN)
		{
			TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_PKGR_LEN_ERR);return;	
		}
		xor=databuf[packlen-1];
		for(i=0;i<packlen-1;i++)
			xor^=databuf[i];
		if(xor)
		{
			TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_CHECK_ERROR);	//��Count������У�����
			return;
		}
        #if 0
		result=Boot_ProgramFlash(databuf,DB_START_ADDR+(Count-1)*REAL_PRO_PKG_LEN,REAL_PRO_PKG_LEN);	
        #else
        memcpy(pUpg+(Count-1)*REAL_PRO_PKG_LEN,databuf,REAL_PRO_PKG_LEN);
        result=TA_SUCCESS;
        #endif
		if(result!=TA_SUCCESS)
		{
			TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_FLASH_ERR);	//��Count������У�����
			return;
		}	
		len-=REAL_PRO_PKG_LEN;
		if(len==0)
		{
			TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_SUCCESS);
			break;
		}
		if(len<0)
		{
			TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_FAIL);
			return;
		}
		TA_AckCommand(CMD_PROG_UPGRADE,Count>>8,Count&0x00ff,TA_SUCCESS);
	}
	//���ݴ����ʽ��ȫ��ȷ
	if(addr==APP_PROG_START)
	{
		Boot_EraseFlashSector(4);
        Boot_EraseFlashSector(5);
        #if 0
		result=Boot_ProgramFlash((uint8 *)DB_START_ADDR,APP_PROG_START,bakeup_Len);
        #else
        result=Boot_ProgramFlash(updatabin,APP_PROG_START,bakeup_Len);
        #endif
	}
	if(result!=TA_SUCCESS)
	{
		TA_AckCommand(CMD_PROG_UPGRADE,FINISHED_UPG,0,TA_FLASH_ERR);	//��Count������У�����
		return;
	}	
	TA_AckCommand(CMD_PROG_UPGRADE,FINISHED_UPG,0,TA_SUCCESS);
    Enable_SysTick();
	Boot_SysReset();				/*ϵͳ��λ*/

}

void TA_AckCommand(uint8 cmd,uint8 var1,uint8 var2,uint8 result)
{
        uint8 data[8];
        data[0]=0xf5;
        data[1]=cmd;
        data[2]=var1;
        data[3]=var2;
        data[4]=result;
        data[5]=0;
        data[6]=data[1]^data[2]^data[3]^data[4];
        data[7]=0xf5;
        Boot_UsartSend(data,8);
}


void TA_GetCommand(unsigned char *cmd)
{
    uint16 	i,uart_data_len=0;	
    uint8	cmd_buf[RX_DATA_BUFLEN];
    memset(cmd_buf,0x00,sizeof(cmd_buf));
    while(1)
    {
        uart_data_len+=Boot_UsartGet(cmd_buf+uart_data_len,1,1);
        if(uart_data_len>=7)
        {// ���ٽ��յ���7�ֽڣ����Կ�ʼ�ж�������
            for(i=0;i<uart_data_len;i++)
            if(cmd_buf[i]==0xf5)
            {
                if(uart_data_len-i>=7)	// ���ҿ�����һ�����Ϲ��������
                {
                    if(cmd_buf[i+1]!=0xf5&&cmd_buf[i+6]== (cmd_buf[i+1]^cmd_buf[i+2]^cmd_buf[i+3]^cmd_buf[i+4]^cmd_buf[i+5]))
                    {
                        memcpy(cmd,cmd_buf+i,7);// ����ֻ�ж�7�ֽ�
                        cmd[7]=0xf5;// Ĭ�ϲ���
                        if(cmd[1]>0x40&&cmd[1]<0x80)
                        {// ���ϴ�������������к��������ݴӴ��ڽ���
                            if(uart_data_len-i<8) 
                            break;
                        // ���������һ��������ܷ��أ���ֹ�����������ݴ�λ
                        }
                    return;
                    }
                }
                if(i>=6) // ���������һ�����Ϲ��������
                {
                    if(cmd_buf[i-6]!=0xf5&&cmd_buf[i-1]==(cmd_buf[i-6]^cmd_buf[i-5]^cmd_buf[i-4]^cmd_buf[i-3]^cmd_buf[i-2]))
                    {
                        memcpy(cmd+1,cmd_buf+i-6,7);// ���󿽱�7�ֽ�
                        cmd[0]=0xf5;//Ĭ�ϲ���
                        return;
                    }
                    else 
                    {// ��ߵ����ݾͲ��������� ����
                        memcpy(cmd_buf,cmd_buf+i,uart_data_len-i);
                        uart_data_len-=i;
                    }
                    }
                }
            }
        if(uart_data_len==RX_DATA_BUFLEN)
        {// ��������������û���յ�����ʱ����λ������
            uart_data_len=0;
        }
    }

}







