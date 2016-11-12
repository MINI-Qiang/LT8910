/*
软件版权所有：深圳市安信可科技有限公司		2013年11月27日发行

中国最大最便宜的 的2.4G模块 生产原厂

技术支持热线：4008 555 368  0755-61195776  15323435161  赵工

批量报价：10K以下2.6RMB 50K以上2.5RMB

特性：128个频道可调  空旷距离120-150米  低功耗   

官网：http://www.ai-thinker.com

厂址：深圳 宝安 西乡 固戍 恒南路 新园工业区六号四楼 安信可科技
*/
#ifndef _SPI_H
#define _SPI_H

#include "stc15l204ea.h"

#define WRITE		0x7F
#define READ		0x80

sbit	TESTLED	= P3^7;
sbit	TXLED	= P1^7;
sbit	RXLED	= P1^6;

sbit	RESET_N	= P1^5;  			//output
sbit	SS 		= P1^4;       		//output
sbit	MOSI 	= P1^3;     		//output
sbit	SCLK	= P1^2;     		//output
sbit	PKT 	= P1^1;  			//input
sbit	MISO 	= P1^0;       		//input

void InitLT8900(void);
void spiWriteReg(unsigned char reg, unsigned char byteH, unsigned char byteL);
void spiReadreg(unsigned char reg);
unsigned char spiReadWrite(unsigned char Byte);

#endif
