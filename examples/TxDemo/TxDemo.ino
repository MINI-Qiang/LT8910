/*
 * 发信机
 * 迷你强
 * chenglei1234@gmail.com
   Copyright (C) 2016 迷你强增补
 Copyright (C) 2015 Rob van der Vee"));rob.c.veer@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/* Connections:

 OCROBOT -> LT8910

 GND        GND
 3v3        VCC
 23         PKT
 21         CS
 24         RST
 11         MOSI
 12         MISO
 13         SCK

*/

#include <SPI.h>
#include "LT8900.h"

const uint8_t PIN_NRF_RST = 24;
const uint8_t PIN_NRF_CS = 21;
const uint8_t PIN_NRF_PKT = 23;


bool writer;

LT8900 lt(PIN_NRF_CS, PIN_NRF_PKT, PIN_NRF_RST);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("\n\nLT8900 module sample, v0.1.\n\n"));
 /*SPI 初始化*/
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  delay(500);
/*2.4G 射频初始化*/
  lt.begin();  //收发机初始化
  lt.setCurrentControl(15,15);  //配置发射功率（0为最大功率输出）
  lt.setDataRate(LT8900::LT8910_62KBPS);  //配置速率（LT8910_62KBPS,LT8910_125KBPS,LT8910_250KBPS,LT8900_1MBPS）,速率越高，通讯距离会越短
  lt.setChannel(0x06);     //配置信道（0.5Mhz为一个间隔从2402Mhz起）
}

void loop()
{
    delay(2000);  //发送时间间隔2秒
    uint8_t data[] = { 'h','e','l','l','o' };  //创建数据包数组
    lt.sendPacket(data, 5);  //将数据封包 并发送
    lt.whatsUp(Serial);   //显示调试信息到指定接口（实际使用时请注释掉）
 }
