/*
 Copyright (C) 2015 Rob van der Veer, <rob.c.veer@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/* Connections:

 Arduino -> LT8900

 GND        GND
 3v3        VCC
 8          PKT
 9          CS
 10         RST
 11         MOSI
 12         MISO
 13         SCK
*/

#include <SPI.h>
#include "LT8900.h"

const uint8_t PIN_NRF_RST = 24;
const uint8_t PIN_NRF_CS = 21;
const uint8_t PIN_NRF_PKT = 23;
const uint8_t PIN_ROLE = A3;    //收发功能切换IO

bool writer;

LT8900 lt(PIN_NRF_CS, PIN_NRF_PKT, PIN_NRF_RST);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.println(F("\n\nLT8900 scanner sample, v0.2.\n\n"));

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV4);

  lt.begin();
  lt.setCurrentControl(15,15);
//  lt.setDataRate(LT8900::LT8910_62KBPS);
  lt.setChannel(0x06);

  lt.whatsUp(Serial);
  Serial.println(F("Boot completed."));
}

void loop()
{
  uint16_t wifi_Channels[] = {
    2412,
    2417,
    2422,
    2427,
    2432,
    2437,
    2442,
    2447,
    2452,
    2457,
    2462,
    2467,
    2472,
    2484
  };
  
  uint16_t signals[32];
  char sbuf[64];
  
  Serial.println("\n\n");
  
  for(int i = 0; i < sizeof(wifi_Channels)/sizeof(uint16_t); i++)
  {  
    lt.scanRSSI(&signals[i], wifi_Channels[i]-2402, 1); 
  }
    
  for(int i = 0; i < sizeof(wifi_Channels)/sizeof(uint16_t); i++)
  {
    sprintf(sbuf, "[%02d] %d = %04x ", i+1, wifi_Channels[i], signals[i]);
    Serial.print(sbuf);    
    for(int j =0 ; j < signals[i]/2; j++)
    {
      Serial.print('#');
    }
    Serial.println();
  }
  
//  
//  Serial.print("Current strength = ");
//  Serial.println(lt.getRSSI());
//  
  delay(250);
 }
