/*
 * 收信机
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
  lt.setChannel(0x06);  //配置信道（500khz为一个间隔从2402Mhz起）
}

void loop()
{
    if (lt.available())  //判断缓冲区是否有数据
    {
      uint8_t buf[32];  //创建收信缓冲区（32字节）
      int packetSize = lt.read(buf, 32);   //获取取信息长度，并且将数据存入缓冲区
      if (packetSize > 0)   //判断数据包是否正常，如果-1则CRC校验出错建议抛弃数据包
      { 
        Serial.println(F("Packet read OK"));

        for(int i = 0; i < packetSize; i++)   //循环输出数组数据
        {
          Serial.print((char)buf[i]);
        }
        Serial.println();
      }
      else
      {
        Serial.println(F("CRC is ERROR"));
      }
      lt.startListening();   //切换到接收模式
    }
  }