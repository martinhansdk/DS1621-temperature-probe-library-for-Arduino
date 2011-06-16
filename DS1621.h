#include <Wire.h>

// based on code from McPhalen published at http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1198065647

// SDA pin is Analog4
// SCL pin is Analog5

// device ID and address

class DS1621 {
  byte addr;

  DS1621(byte i2c_addr);

  // Set configuration register
  void setConfig(byte cfg);

  // Read a DS1621 register
  byte getReg(byte reg);

  // Sets temperature threshold
  // -- whole degrees C only
  // -- works only with ACCESS_TL and ACCESS_TH
  void setThresh(byte reg, int tC);

  // Start/Stop DS1621 temperature conversion
  void startConversion(boolean start);

  // Reads temperature or threshold
  // -- whole degrees C only
  // -- works only with RD_TEMP, ACCESS_TL, and ACCESS_TH
  int getTemp(byte reg);

  // Read high resolution temperature
  // -- returns temperature in 1/100ths degrees
  // -- DS1620 must be in 1-shot mode
  int getHrTemp();
}



