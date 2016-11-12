/*
������Ȩ���У������а��ſɿƼ����޹�˾		2013��11��27�շ���

�й����������˵� ��2.4Gģ�� ����ԭ��

����֧�����ߣ�4008 555 368  0755-61195776  15323435161  �Թ�

�������ۣ�10K����2.6RMB 50K����2.5RMB

���ԣ�128��Ƶ���ɵ�  �տ�����120-150��  �͹���

������http://www.ai-thinker.com

��ַ������ ���� ���� ���� ����· ��԰��ҵ��������¥ ���ſɿƼ�
*/
#include "spi.h"
#include "delay.h"

unsigned char RegH, RegL;

//-----------------------------------------------------------------------------
unsigned char spiReadWrite(unsigned char Byte)
{
    unsigned char i;

	EA = 0;
    for (i = 0; i < 8; i++)
    {
        MOSI = (Byte & 0x80);
		SCLK = 1;
		Delay10us();
        Byte <<= 1;
        Byte |= MISO;
        SCLK = 0;
		Delay10us();
    }
	EA = 1;

    return (Byte);
}

//----------------------------------------------------------------------------
void spiReadreg(unsigned char reg)
{
	SS = 0;
    spiReadWrite(READ | reg);
    RegH = spiReadWrite(0x00);
    RegL = spiReadWrite(0x00);
	SS = 1;
}

//----------------------------------------------------------------------------
void spiWriteReg(unsigned char reg, unsigned char byteH, unsigned char byteL)
{
	SS = 0;
    spiReadWrite(WRITE & reg);
    spiReadWrite(byteH);
    spiReadWrite(byteL);
	SS = 1;
}

//----------------------------------------------------------------------------
void InitLT8900(void)
{
    RESET_N = 0;
    delayMs(100);
    RESET_N = 1;
    delayMs(200);
    SCLK = 0;

    spiWriteReg(0, 0x6f, 0xe0);
    spiWriteReg(1, 0x56, 0x81);
    spiWriteReg(2, 0x66, 0x17);
    spiWriteReg(4, 0x9c, 0xc9);
    spiWriteReg(5, 0x66, 0x37);
    spiWriteReg(7, 0x00, 0x00);
    spiWriteReg(8, 0x6c, 0x90);
    spiWriteReg(9, 0x48, 0x00);				// 5.5dBm
    spiWriteReg(10, 0x7f, 0xfd);
    spiWriteReg(11, 0x00, 0x08);
    spiWriteReg(12, 0x00, 0x00);
    spiWriteReg(13, 0x48, 0xbd);

    spiWriteReg(22, 0x00, 0xff);
    spiWriteReg(23, 0x80, 0x05);
    spiWriteReg(24, 0x00, 0x67);
    spiWriteReg(25, 0x16, 0x59);
    spiWriteReg(26, 0x19, 0xe0);
    spiWriteReg(27, 0x13, 0x00);
    spiWriteReg(28, 0x18, 0x00);

    spiWriteReg(32, 0x50, 0x00);
    spiWriteReg(33, 0x3f, 0xc7);
    spiWriteReg(34, 0x20, 0x00);
    spiWriteReg(35, 0x03, 0x00);
    spiWriteReg(36, 0x03, 0x80);
    spiWriteReg(37, 0x03, 0x80);
    spiWriteReg(38, 0x5a, 0x5a);
    spiWriteReg(39, 0x03, 0x80);
    spiWriteReg(40, 0x44, 0x01);
    spiWriteReg(41, 0xB0, 0x00);  //crc on scramble off ,1st byte packet length ,auto ack off
    spiWriteReg(42, 0xfb, 0xb0);  //
    spiWriteReg(43, 0x00, 0x0f);
    spiWriteReg(50, 0x00, 0x00);
	delayMs(200);

    spiWriteReg(7, 0x01, 0x00);
	delayMs(2);
    spiWriteReg(7, 0x00, 0x30);
}
