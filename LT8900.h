/*
 Copyright (C) 2015 Rob van der Veer, <rob.c.veer@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef LT8900_H
#define LT8900_H

class LT8900
{

  public:
    /** Data Data
    * While the LT8900 only has support for 1MPBS speed, the LT8910 offers different speeds.
    * @see getIs8910
    */
    enum DataRate
    {
      LT8900_1MBPS,     /** default transmit rate */
      LT8910_250KBPS,   /** 250 Kpbs, only on lt8910 */
      LT8910_125KBPS,   /** 125 Kbps, only on lt8910 */
      LT8910_62KBPS     /** 62 Kbps, only on lt8910 */
    };

  private:
    uint8_t _pin_chipselect;
    uint8_t _pin_pktflag;
    uint8_t _pin_reset;
    uint8_t _channel;
    bool _isLT8910;

  public:
    /** Construct a new instance
     * @param cs Chip Select, this is the SLAVE SELECT pin. Please note that it is a different location than the NRF24L01+ SS pin.
     * @param pkt PKT_flag input signal pin. Comparable to the NRF24L01+ IRQ pin.
     * @param rst RESET pin. Use 0 to disable.
    */
    LT8900(const uint8_t cs, const uint8_t pkt, const uint8_t rst);

    /** Configures the module for initial use */
    void begin();

    /** 设置信道
    * @param channel has significant 7 bits
    */
    void setChannel(uint8_t channel);
    /** Retrieve the current channel */
    uint8_t getChannel();

    /** Set power and gain
    * @param power 0-0xf
    * @param gain 0-0xf
    */
    void setCurrentControl(uint8_t power, uint8_t gain);

    /** Sets the data rate
    * @param rate the transmission/reception speed
    * @returns true when the data rate was succesfully changed.
    */
    bool setDataRate(DataRate rate);

    /** Returns the data rate
    * @returns the active data rate.
    */
    DataRate getDataRate();

    /** Read the value of a register
    * @param reg
    */
    uint16_t readRegister(uint8_t reg);

    /** Writes to a register
    * @param reg The register to write to
    * @param data 16bits of data, MSB first */
    uint8_t writeRegister(uint8_t reg, uint16_t data);

    /** Writes to a register, one byte at a time.
    * This is a convenience function, because the LT8900 uses 16 bits registers with all 16 bits written at once.
    * @param reg The register to write to
    * @param high bits 15..8
    * @param low bits 7..0  */
    uint8_t writeRegister2(uint8_t reg, uint8_t high, uint8_t low);

    /** Put the LT8900 module to sleep mode.
     * In contrast to POWER DOWN, this mode will keep the register values
     * Any SPI call will awaken the module automatically */
    void sleep();

    /** Dumps debug information on the selected port
    * @param stream the output stream to use as output, eg. `whatsUp(&Serial);`
    */
    void whatsUp(Stream &stream);

    /** Signals a packet ready in the FIFO queue */
    bool available();

    /** Checks the module type
    * The LT8910 has a few extra features one of it being the extended options for data rate.
    * @returns false for a regular LT8900 and true for a LT8910
    * @see setDataRate
    */
    bool getIs8910();

    /** Read a packet into the buffer
    * @param buffer a pointer to a buffer large enough to hold the packet
    * @param maxBuffer the maximum size of the buffer. Any bytes left over in the buffer will be dropped.
    * @returns the size of the packet that was read, -1 for CRC error
    */
    int read(uint8_t *buffer, size_t maxBuffer);

    /** Switch the module to RX mode */
    void startListening();

    /** Sets the internal clock divider
   * @param clock
   */
    void setClock(uint8_t clock);

    /** Sends a packet
    * This call blocks until the packet was sent and the Tx Buffer has been completed
    * @param data
    * @param packetSize
    * @returns true when the packet was sent, or false when the packetSize was invalid.
    */
    bool sendPacket(uint8_t *data, size_t packetSize);

    /** Set Syncword
    * @param syncWord 64 bits of sync word settings.
    * @see setSyncwordLength
    */
    void setSyncWord(uint64_t syncWord);

    /** Sets the length of the sync word used
     * @param length:
     *          11 = 64bits, 10 = 48 bits, 01 = 32 bits, 00 = 16 bits
     * @see Check the datasheet for which bits are actually used.
     */
    void setSyncWordLength(uint8_t length);

    /** Scan the signal strength for one or more channels
    * @param buffer points to a buffer to store the signal strengths, at least num_channels
    * @param start_channel starting channel to scan, where Frequency = 2402 + channel
    * @param num_channels number of channels to scan in this batch, for example scanning 4 channels
    *                     with start 2480 will scan 2480, 2481, 2482, 2483
    */
    void scanRSSI(uint16_t *buffer, uint8_t start_channel, uint8_t num_channels);

    /** retrieve the analog signal strength for the current channel */
    uint8_t getRSSI();
};


#endif //LT8900_H
