#include <DS1621.h>

// DS1621 demo
// based on code from McPhalen published at http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1198065647

// SDA pin is Analog4
// SCL pin is Analog5
// DS1621 has A2, A1, and A0 pins connected to GND

void setup()
{
  byte addr = (0x90 >> 1) | 0;  // replace the 0 with the value you set on pins A2, A1 and A0
  DS1621 sensor=DS1621(addr);

  DS1621.startConversion(false);                       // stop if presently set to continuous
  DS1621.setConfig(POL | ONE_SHOT);                    // Tout = active high; 1-shot mode
  DS1621.setThresh(ACCESS_TH, 27);                     // high temp threshold = 80F
  DS1621.setThresh(ACCESS_TL, 24);                     // low temp threshold = 75F

  Serial.begin(9600);
  delay(5);
  Serial.println("DS1621 Demo");

  int tHthresh = DS1621.getTemp(ACCESS_TH);
  Serial.print("High threshold = ");
  Serial.println(tHthresh);

  int tLthresh = DS1621.getTemp(ACCESS_TL);
  Serial.print("Low threshold = ");
  Serial.println(tLthresh);
}


void loop()
{
  int tC, tFrac;
  
  tC = DS1621.getHrTemp();                             // read high-resolution temperature
  
  if (tC < 0) {
    tC = -tC;                                   // fix for integer division
    Serial.print("-");                          // indicate negative
  }
  
  tFrac = tC % 100;                             // extract fractional part
  tC /= 100;                                    // extract whole part
  
  Serial.print(tC);
  Serial.print(".");
  if (tFrac < 10)
    Serial.print("0");
  Serial.println(tFrac);
  
  delay(500);
}

