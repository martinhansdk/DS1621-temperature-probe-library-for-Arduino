#include "Arduino.h"
#include "DS1621.h"
#include <Wire.h>
// based on code from McPhalen published at http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1198065647



DS1621::DS1621(uint8_t i2c_addr) {
  addr=i2c_addr;
  Wire.begin();
}

// device ID and address
#define DEV_TYPE   0x90 >> 1                    // shift required by wire.h
#define DEV_ADDR   0x00                         // DS1621 address is 0
#define SLAVE_ID   DEV_TYPE | DEV_ADDR

// Set configuration register

void DS1621::setConfig(uint8_t cfg)
{
 Wire.beginTransmission(SLAVE_ID);
 Wire.write(ACCESS_CFG);
 Wire.write(cfg);
 Wire.endTransmission();
 delay(15);                                    // allow EE write time to finish
}


// Read a DS1621 register

uint8_t DS1621::getReg(uint8_t reg)
{
 Wire.beginTransmission(SLAVE_ID);
 Wire.write(reg);                               // set register to read
 Wire.endTransmission();
 Wire.requestFrom(SLAVE_ID, 1);
 uint8_t regVal = Wire.read();
 return regVal;
}


// Sets temperature threshold
// -- whole degrees C only
// -- works only with ACCESS_TL and ACCESS_TH

void DS1621::setThresh(uint8_t reg, int tC)
{
 if (reg == ACCESS_TL || reg == ACCESS_TH) {
   Wire.beginTransmission(addr);
   Wire.write(reg);                             // select temperature reg
   Wire.write(uint8_t(tC));                        // set threshold
   Wire.write((uint8_t)0);                               // clear fractional bit
   Wire.endTransmission();
   delay(15);
 }
}


// Start/Stop DS1621 temperature conversion

void DS1621::startConversion(bool start)
{
 Wire.beginTransmission(addr);
 if (start == true)
   Wire.write(START_CNV);
 else
   Wire.write(STOP_CNV);
 Wire.endTransmission();
}


// Reads temperature or threshold
// -- whole degrees C only
// -- works only with RD_TEMP, ACCESS_TL, and ACCESS_TH

int DS1621::getTemp(uint8_t reg)
{
 if (reg == RD_TEMP || reg == ACCESS_TL || reg == ACCESS_TH) {
   int tC;
   uint8_t tVal = getReg(reg);
   if (tVal >= B10000000) {                    // negative?
     tC = 0xFF00 | tVal;                       // extend sign bits
   }
   else {
     tC = tVal;
   }
   return tC;                                  // return threshold
 }
 return 0;                                     // bad reg, return 0
}


// Read high resolution temperature
// -- returns temperature in 1/100ths degrees
// -- DS1620 must be in 1-shot mode

int DS1621::getHrTemp()
{
 startConversion(true);                        // initiate conversion
 uint8_t cfg = 0;
 while (!(cfg & DONE)) {                          // let it finish
   cfg = getReg(ACCESS_CFG);
 }

 int tHR = getTemp(RD_TEMP);                   // get whole degrees reading
 uint8_t cRem = getReg(RD_CNTR);                  // get counts remaining
 uint8_t slope = getReg(RD_SLOPE);                // get counts per degree

 if (tHR >= 0)
   tHR = (tHR * 100 - 25) + ((slope - cRem) * 100 / slope);
 else {
   tHR = -tHR;
   tHR = (25 - tHR * 100) + ((slope - cRem) * 100 / slope);
 }
 return tHR;
}
