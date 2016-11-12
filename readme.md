LT8900lib
=========
---------------miniqiang----------------
Add RXdemo , TXdeom use on OCROBOT 8F328D-RF
http://www.ocrobot.com/doku.php?id=zh:ocrobot:alpha:8f328d-rf:mian
----------------------------------------

Rob van der Veer, may 2015.

This is driver library for use with the low cost LT8900 and LT8910 2.4Ghz transceiver modules as found on ebay.

This library was written because I could not found one, and the chips are incompatible with the NRF24L01(+) modules, even though they look very similar.

###Installation

To install, get the .ZIP from here:

[https://bitbucket.org/robvanderveer/lt8900lib/downloads]()

When downloaded, rename the zip to `LT8900.zip` and use the Arduino IDE to install the library.

###Class documentation

The folder /doc contains generated html documentation.

###Connections

> modules are 3.3v maximum! Use a 3.3v Arduino or provide sufficient level shifting (e.g. via 74HC4050)

Pinout of the module looks like NRF24L01 but they are different. Below is a diagram to illustrate:

| 1 | 2 |
|---|---|
| 3 | 4 |
| 5 | 6 |
| 7 | 8 |


IN | LT8900 | NRF24L01
---|--------|--------
1 |  PKT 	  | IRQ
2 |  MISO	  | MISO
3 | MOSI 	  | MOSI
4 | SCK	  | SCK
5 | RESET   | **SS**
6 | **SS**  | CE
7 | VCC     | VCC
8 | GND     | GND

On the LT8900, pin 5 should not be connected, because that one is tied to the LDO out of the module.

###Simplyduino compatibility
Simplyduino is a custom board I developed especially for hosting these 2.4Ghz modules. All information can be found on the project site [here](http://simplicate.info/simplyduino).

The pinouts are compatible but there is a caviat. On the simplyduino pin 5 of the 2x4 header is connected to the Slave Select #10 pin of the AtMega. The Arduino SPI.h always uses this pin as an OUTPUT pin, and automatically sets it to `HIGH`. The LT8900 module uses this very pin as a RESET pin. Pretty handy, if you are aware.

> One of my LT8900 boards came with a bad connection, shorting the RESET pin with the VDD pin on the CPU which caused all sorts of drama. 

On other boards, just leave the pin disconnected.

###Module workings
These LT8900 networking chips are a little simpler than the NRF. They don't have *shockburst* or *pipes* or any of that fancy stuff.

When you send a packets, the packet assembler adds leaders and trailers and CRC checks etc. One of the things that is added to a package is a *SyncWord*. A module that is listening for incoming data will only read the payload when the syncword matches. So basically, you can create multiple pipes manually by setting the syncword before a LT8900::sendPacket, and sertting the same syncword on the receiver side just before the LT8900::startListening call. This can come in handy when you want to do a send/reply protocol with multiple nodes.

> This is in contrast with the NRF24L01 which can listen to multiple (5) pipes at the same time.

> Note that if you are switching syncwords, you will not be able to receive other packets. The will not be queued.

The LT8900 is very open with respect to packet sizes. Because of the packet length byte, odd packet sizes are preferred (1,3,5,7, etc). The fifo can handle packets up to 64 bytes.

###Sending data
To send a packet, simply call LT8900::sendPacket.

###Receiving data
Reading and sending states are mutually exclusive.

To start receiving data, put out a call to LT8900::startListening and regularly poll the LT8900::available method. If you are fancy, you could add an interrupt to the PKT_flag pin (which does basically the same). When a packet is waiting (LT8900::available = true), call LT8900::read to retrieve the data from the FIFO queue. To receive the next payload, you must call LT8900::startListening() again.

###RSSI scanning

Retrieve the signal strength of one or more channels by calling LT8900::scanRSSI(). While the module can scan 128 channels (7 bits) only 6 bits are available for the number of consequetive channels so when you want to scan all channels you will have to do it it batches. The reason for this is the size of the FIFO queue that can only handle 64 bytes maximum.

Use LT8900::getRSSI() to retrieve the signal strength for the active channel. 

###Examples

- the example `lt8900test` shows the basic usage.
- the second sample `scanner` shows the activity on the Wifi Channels.
