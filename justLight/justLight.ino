// ParticleBots
// Karolina Ladino Puerto
// Thesis of Automation Engineering
// June 2017 V1.0
// Utadeo

// Note: Based in an public example of light sensor TSL2561
// Libraries
#include <Wire.h>
#include "TSL2561.h"

// Addr 0x39
TSL2561 tsl(TSL2561_ADDR_FLOAT); 

// Setup
void setup(void) {
  Serial.begin(9600);
  
  if (tsl.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No sensor?");
    while (1);
  }
    
  // Adapt the gain
  //tsl.setGain(TSL2561_GAIN_0X);         // 0 - no gain (bright situations)
  tsl.setGain(TSL2561_GAIN_16X);      // 16x - gain (dim situations)
  
  // Time of measure
  tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // Shortest integration time (bright light)
  //tsl.setTiming(TSL2561_INTEGRATIONTIME_101MS);  // Medium integration time (medium light)
  //tsl.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // Longest integration time (dim light)
}

// Loop
void loop(void) {
  // Reading
  //uint16_t x = tsl.getLuminosity(TSL2561_VISIBLE);     
  //uint16_t x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
  uint16_t x = tsl.getLuminosity(TSL2561_INFRARED);
  
  Serial.println(x, DEC);

  // Advaced data read. 32 bits (12 of IR and 12 of fullspectrum
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
  Serial.print("Full: "); Serial.print(full);   Serial.print("\t");
  Serial.print("Visible: "); Serial.print(full - ir);   Serial.print("\t");
  Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));
  
  delay(500); 
}
