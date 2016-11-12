/*
Software Copyright: Shenzhen Shun Technology Co., Ltd. can be November 27, 2013 issue

China&#39;s largest and most expensive of 2.4G module production factory

Technical support hotline: 0755-61195776 15323435161 4008555368 Zhao workers

Bulk Quote: 10K less than 2.5RMB 2.6RMB 50K

Features: 128 channels adjustable open distance 120-150 meters Low power consumption

Official website: http: //www.ai-thinker.com

Address: Shenzhen Baoan Xixiang Hang Road, the fourth floor of the new industrial park area on the 6th Shun available Technology
*/

#include &quot;spi.h&quot;
#include &quot;delay.h&quot;

#define IDLE 0
#define STANDBY 1
#define RECEIVE 2
#define SENDDATA 3
#define TESTPA 4
#define ACK 5
#define NO_CHANGE 255

extern unsigned char RegH, RegL;
extern bit fTimer1ms;
bit fTestKey;

unsigned char smState, keyState, ackOk;
unsigned char keyData, keyTrg, keyCont, keyTime;

unsigned char ScanKey (void);

// ------------------------------------------------ -----------------------------
void main (void)
{
	P1 = 0xFF;
	P1M1 = 0x03;
	P1M0 = 0xFC;

	Timer1Init ();
	InitLT8900 ();

	smState = STANDBY;

	while (1)
	{
		switch (smState)
		{
			case RECEIVE:
			if (1 == PKT)
			{
				smState = STANDBY;
				spiReadreg (48);
				if (0x00 == (RegH &amp; 0x80))
				{
					spiReadreg (50);
					ackOk = RegL;
					spiReadreg (50);
					spiReadreg (50);

					TESTLED = 0;
					delayMs (200);
					TESTLED = 1;
					smState = STANDBY; // ACK;
				}
			}
			break;

			case STANDBY:
			spiWriteReg (7, 0x00, 0x30);
			delayMs (3);
			spiWriteReg (52, 0x00, 0x80); // clear the receive buffer
			spiWriteReg (7, 0x00, 0xB0); // allows the receiver to enable
			delayMs (5);
			smState = RECEIVE;
			break;

			case IDLE:
			break;
		}

		keyState = ScanKey ();
		switch (keyState)
		{
			case SENDDATA: // data transmission test
			TXLED = 0;
			RXLED = 1;

			if (fTestKey == 1)
			{
				spiWriteReg (32, 0x50, 0x00);
				spiWriteReg (34, 0x20, 0x00);
				spiWriteReg (11, 0x00, 0x08);
				fTestKey = 0;
			}

			spiWriteReg (7, 0x00, 0x00);
			spiWriteReg (52, 0x80, 0x00); // flush the transmit buffer zone

			// Send 5 bytes
			spiWriteReg (50, 5, 0x55);
			spiWriteReg (50, 2, 3);
			spiWriteReg (50, 4, 5);

			spiWriteReg (7, 0x01, 0x30); // allow emission enable

			while (PKT == 0);
			smState = STANDBY;

			delayMs (200);
			TXLED = 1;
			break;

			case TESTPA: // Carrier Test
			TXLED = 1;
			RXLED = 0;
			InitLT8900 ();

			if (fTestKey == 0)
			{
				spiWriteReg (7, 0x00, 0x30);
				delayMs (3);
				spiWriteReg (32, 0x18, 0x07);
				spiWriteReg (34, 0x83, 0x0B);
				spiWriteReg (11, 0x80, 0x08);
				spiWriteReg (7, 0x01, 0x30);
				fTestKey = 1;
				smState = IDLE;
			}
			else
			{
				spiWriteReg (32, 0x50, 0x00);
				spiWriteReg (34, 0x20, 0x00);
				spiWriteReg (11, 0x00, 0x08);
				spiWriteReg (7, 0x00, 0x30);
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

// ------------------------------------------------ -----------------------------
unsigned char ScanKey (void)
{
	unsigned char nextState = NO_CHANGE;

	keyData = P3;
	keyData = ~ (keyData &amp; 0x03);
	keyTrg = keyData &amp; (keyData ^ keyCont);
	keyCont = keyData;

	if (keyCont &amp; 0x01)
	nextState = SENDDATA;
	if (keyTrg &amp; 0x02)
	nextState = TESTPA;

	return (nextState);
}
</pre>
