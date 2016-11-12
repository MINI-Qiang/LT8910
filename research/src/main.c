/*
软件版权所有：深圳市安信可科技有限公司		2013年11月27日发行

中国最大最便宜的 的2.4G模块 生产原厂

技术支持热线：4008 555 368  0755-61195776  15323435161  赵工

批量报价：10K以下2.6RMB 50K以上2.5RMB

特性：128个频道可调  空旷距离120-150米  低功耗   

官网：http://www.ai-thinker.com

厂址：深圳 宝安 西乡 固戍 恒南路 新园工业区六号四楼 安信可科技
*/
#include "spi.h"
#include "delay.h"

#define	IDLE		0
#define STANDBY		1
#define RECEIVE		2
#define SENDDATA	3
#define TESTPA		4
#define ACK			5
#define NO_CHANGE   255

extern unsigned char RegH, RegL;
extern bit fTimer1ms;
bit fTestKey;

unsigned char smState, keyState, ackOk;
unsigned char keyData, keyTrg, keyCont, keyTime;

unsigned char ScanKey(void);

//-----------------------------------------------------------------------------
void main(void)
{
	P1 = 0xFF;
	P1M1 = 0x03;
	P1M0 = 0xFC;

	Timer1Init();
	InitLT8900();

	smState = STANDBY;

	while (1)
	{
		switch (smState)
		{
			case RECEIVE:
					if (1 == PKT)
					{
						smState = STANDBY;
						spiReadreg(48);
						if (0x00 == (RegH & 0x80))
						{
							spiReadreg(50);
							ackOk = RegL;
							spiReadreg(50);
							spiReadreg(50);

							TESTLED = 0;
							delayMs(200);
							TESTLED = 1;
							smState = STANDBY;	//ACK;
						}
					}
					break;

			case STANDBY:
					spiWriteReg(7, 0x00, 0x30);
					delayMs(3);
					spiWriteReg(52, 0x00, 0x80);			// 清接收缓存区
					spiWriteReg(7, 0x00, 0xB0);				// 允许接收使能
					delayMs(5);					
					smState = RECEIVE;
					break;

		 	case IDLE:
					break;
		}

		keyState = ScanKey();
		switch (keyState)
		{
			case SENDDATA:	 // 数据发送测试
					TXLED = 0;
					RXLED = 1;

					if (fTestKey == 1)
					{
						spiWriteReg(32, 0x50, 0x00);
						spiWriteReg(34, 0x20, 0x00);
						spiWriteReg(11, 0x00, 0x08);
						fTestKey = 0;
					}

					spiWriteReg(7, 0x00, 0x00);
					spiWriteReg(52, 0x80, 0x00);			// 清空发送缓存区

					// 发送5个字节
					spiWriteReg(50, 5, 0x55);
					spiWriteReg(50, 2, 3);
					spiWriteReg(50, 4, 5);

					spiWriteReg(7, 0x01, 0x30);				// 允许发射使能
					
					while (PKT == 0);
					smState = STANDBY;

					delayMs(200);
					TXLED = 1;
					break;

			case TESTPA:	// 载波测试
					TXLED = 1;
					RXLED = 0;
					InitLT8900();

				 	if (fTestKey == 0)
				 	{
						spiWriteReg(7, 0x00, 0x30);
						delayMs(3);
				 		spiWriteReg(32, 0x18, 0x07);
				 		spiWriteReg(34, 0x83, 0x0B);
						spiWriteReg(11, 0x80, 0x08);
						spiWriteReg(7, 0x01, 0x30);
						fTestKey = 1;
						smState = IDLE;
				 	}
				 	else
				 	{
						spiWriteReg(32, 0x50, 0x00);
						spiWriteReg(34, 0x20, 0x00);
						spiWriteReg(11, 0x00, 0x08);
						spiWriteReg(7, 0x00, 0x30);
						fTestKey = 0;
						smState = STANDBY;
						RXLED = 1;
				 	}
					break;

			case NO_CHANGE:
					break;
		}

	}
}

//-----------------------------------------------------------------------------
unsigned char ScanKey(void)
{
	unsigned char nextState = NO_CHANGE;
	
	keyData = P3;
	keyData = ~(keyData & 0x03);
	keyTrg = keyData & (keyData ^ keyCont);
	keyCont = keyData;

	if (keyCont & 0x01)
		nextState = SENDDATA;
	if (keyTrg & 0x02)
		nextState = TESTPA;
	
	return (nextState);		
}
