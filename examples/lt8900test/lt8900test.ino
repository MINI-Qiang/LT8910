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

 Connect A3 to ground for sender, leave open for receiver role.
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
  Serial.begin(9600);
  Serial.println(F("\n\nLT8900 module sample, v0.1.\n\n"));

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV4);

  pinMode(PIN_ROLE, INPUT_PULLUP);

  delay(500);

  lt.begin();
  lt.setCurrentControl(4,15);
  lt.setDataRate(LT8900::LT8910_62KBPS);
  lt.setChannel(0x06);

  char sbuf[32];

  //verify chip registers.
  for (int i = 0; i <= 50; i++)
  {
    uint16_t value = lt.readRegister(i);

    sprintf_P(sbuf, PSTR("%d = %04x\r\n"), i, value);
    Serial.print(sbuf);
  }

  if ((digitalRead(PIN_ROLE) == LOW))
  {
      writer = true;
    Serial.println(F("Writer mode"));
  }
  else
  {
      writer = false;
    Serial.println(F("Reader mode"));
    lt.startListening();
  }

  lt.whatsUp(Serial);
  Serial.println(F("Boot completed."));
}

void loop()
{
  if (writer)
  {
    delay(2000);

    //send a packet.
    uint8_t data[] = { 1,2,3,4,5 };

    lt.sendPacket(data, 5);
    lt.whatsUp(Serial);
  }
  else
  {
    if (lt.available())
    {
      Serial.println(F("Data available"));

      uint8_t buf[32];

      int packetSize = lt.read(buf, 32);
      if (packetSize > 0)
      {
        Serial.println(F("Packet read OK"));

        //dump the packet.
        for(int i = 0; i < packetSize; i++)
        {
          Serial.print(i);
          Serial.print("=");
          Serial.println(buf[i]);
        }
      }
      else
      {
        Serial.println(F("Packet read fail"));
        lt.whatsUp(Serial);
      }

      lt.startListening();
    }
  }
}
