// based on code from McPhalen published at http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1198065647

// SDA pin is Analog4
// SCL pin is Analog5

#ifndef DS1621_h
#define DS1621_h

class DS1621 {
  uint8_t addr;

 public:
  DS1621(uint8_t i2c_addr);

  // Set configuration register
  void setConfig(uint8_t cfg);

  // Read a DS1621 register
  uint8_t getReg(uint8_t reg);

  // Sets temperature threshold
  // -- whole degrees C only
  // -- works only with ACCESS_TL and ACCESS_TH
  void setThresh(uint8_t reg, int tC);

  // Start/Stop DS1621 temperature conversion
  void startConversion(bool start);

  // Reads temperature or threshold
  // -- whole degrees C only
  // -- works only with RD_TEMP, ACCESS_TL, and ACCESS_TH
  int getTemp(uint8_t reg);

  // Read high resolution temperature
  // -- returns temperature in 1/100ths degrees
  // -- DS1620 must be in 1-shot mode
  int getHrTemp();


  // DS1621 Registers & Commands
  static const uint8_t RD_TEMP    = 0xAA;        // read temperature register
  static const uint8_t ACCESS_TH  = 0xA1;        // access high temperature register
  static const uint8_t ACCESS_TL  = 0xA2;        // access low temperature register
  static const uint8_t ACCESS_CFG = 0xAC;        // access configuration register
  static const uint8_t RD_CNTR    = 0xA8;        // read counter register
  static const uint8_t RD_SLOPE   = 0xA9;        // read slope register
  static const uint8_t START_CNV  = 0xEE;        // start temperature conversion
  static const uint8_t STOP_CNV   = 0X22;        // stop temperature conversion

  // DS1621 configuration bits
  static const uint8_t DONE       = 0x80; // B10000000   conversion is done
  static const uint8_t THF        = 0x40; // B01000000   high temp flag
  static const uint8_t TLF        = 0x20; // B00100000   low temp flag
  static const uint8_t NVB        = 0x10; // B00010000   non-volatile memory is busy
  static const uint8_t POL        = 0x02; // B00000010   output polarity (1 = high, 0 = low)
  static const uint8_t ONE_SHOT   = 0x01; // B00000001   1 = one conversion; 0 = continuous conversion

};

#endif

