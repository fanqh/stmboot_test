#ifndef _APP_DEFINE_H
#define _APP_DEFINE_H



#define STM32F1_FLASH_BASE		    0x08000000UL
#define STM32F103C8				    0x10000UL	//E 系列 64K
#define STM32F103CB				    0x20000UL	//E 系列 128K
#define STM32F103CC				    0x40000UL	//E 系列 256K
/*----------------------------------------------------------------------
人为定义 扇区 一个区8k
扇区        区域说明        f1xxx flash页段     起始地址
sector 0-1  bootloader      page0-page15        0x08000000UL
sector 2    para1           page16-page23       0x08004000UL
sector 3    para2           page24-page31       0x08006000UL
sector 4-5  m3_app          page32-page47       0x08008000UL
sector 6-7  backup          page48-page63       0x0800c000UL
----------------------------------------------------------------------*/

#define FLASH_LEN				    STM32F103C8
#define BOOTLOADER_START		    STM32F1_FLASH_BASE
#define BOOTLOADER_LEN			    0x4000UL		//bootloader长度 16K 

#define TA_PARAM1_START	            (BOOTLOADER_START+BOOTLOADER_LEN)
#define TA_PARAM1_LEN		        0x2000UL		//参数区 8K 
#define TA_PARAM2_START	            (TA_PARAM1_START+TA_PARAM1_LEN)
#define TA_PARAM2_LEN		        0x2000UL		//参数区 8K 


#define APP_PROG_START			    (TA_PARAM2_START+TA_PARAM2_LEN)
#define APP_PROG_LEN			    0x4000UL		//应用层占空间 16k

#define DB_START_ADDR               (APP_PROG_START+APP_PROG_LEN)
#define DB_LEN                      (0x4000UL)      //备份falsh 16k

#define SECTOR_LEN                  0x2000UL		//参数区 8K 

#define BOOT_VERSIONSIZE            100

//return 
#define TA_SUCCESS                  0
#define TA_FAIL                     1

//upg
#define TA_PRO_FORMAT_ERR			(0x10)		//程序格式错误
#define TA_CHECK_ERROR				(0x11)		//数据校验出错
#define TA_LEN_OVERFLOW				(0x12)		//长度超长
#define TA_ADDR_FORMAT_ERR			(0x13)		//地址格式错误
#define TA_PKGR_LEN_ERR				(0x14)		//数据包长度不对
#define TA_FLASH_ERR				(0x15)		//flash编程出错
#define  FINISHED_UPG			    (0x80)


#define  PROGRAM_LEN_SIGN			(4)

#define  PKG_LEN					(0x78+1)	//120+1
#define  REAL_PRO_PKG_LEN			(0x78)	    //120


//buad id
#define TA_BAUD_9600    			1
#define TA_BAUD_19200				2                          
#define TA_BAUD_38400				3                          
#define TA_BAUD_57600				4                          
#define TA_BAUD_115200				5  

//cmd buf
#define CMD_GET_VERSION				0x26
#define CMD_PROG_UPGRADE		    0x57
#define CMD_AUTHORIZE			    0x98

#endif

