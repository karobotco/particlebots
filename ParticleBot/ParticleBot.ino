// ParticleBots
// Karolina Ladino Puerto
// Thesis of Automation Engineering
// June 2017 V1.0
// Utadeo

// Note: Integration of sensors and magnetometer
// Libraries
#include <Wire.h>
#include "TSL2561.h"
#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

const int sclPin = D1;
const int sdaPin = D2;

// Addr 0x39
TSL2561 tsl(TSL2561_ADDR_FLOAT); 

// Function of read
void MPURead(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  
  // Read Nbytes
  Wire.requestFrom(Address, Nbytes); 
  uint8_t index=0;
  while (Wire.available())
    Data[index++]=Wire.read();
}

// Write a byte (Data) in device (Address) at register (Register)
void MPUWriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}
void MPUBegin(){
    // Configure gyroscope range
  MPUWriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_2000_DPS);
  // Configure accelerometers range
  MPUWriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_16_G);
  // Set by pass mode for the magnetometers
  MPUWriteByte(MPU9250_ADDRESS,0x37,0x02);
  
  // Request first magnetometer single measurement
  MPUWriteByte(MAG_ADDRESS,0x0B,0x01); 
  MPUWriteByte(MAG_ADDRESS,0x0B,0x00); 
  MPUWriteByte(MAG_ADDRESS,0x0A,0x02); // [modo 2 leyendo a 8hz](https://electronics.stackexchange.com/questions/210295/accessing-magnetometer-results-in-mpu-9250)
  }

// Setup
void setup(void) {
  Wire.begin(sdaPin, sclPin);
  Serial.begin(115200);
  
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
  MPUBegin();
}

long int cpt=0;

// Loop
void loop(void) {
  // Reading
  //uint16_t x = tsl.getLuminosity(TSL2561_VISIBLE);     
  //uint16_t x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
  uint16_t x = tsl.getLuminosity(TSL2561_INFRARED);
  // Display data counter
  Serial.print (cpt++,DEC);
  Serial.print (",");
    
  Serial.print(x, DEC);
  Serial.print (",");
 /* // Advaced data read. 32 bits (12 of IR and 12 of fullspectrum
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
  Serial.print("Full: "); Serial.print(full);   Serial.print("\t");
  Serial.print("Visible: "); Serial.print(full - ir);   Serial.print("\t");
  Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));*/
    // Read accelerometer and gyroscope
  uint8_t Buf[14];
  MPURead(MPU9250_ADDRESS,0x3B,14,Buf);
  
  // Create 16 bits values from 8 bits data
  
  // Accelerometer
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t az=Buf[4]<<8 | Buf[5];

  // Gyroscope
  int16_t gx=-(Buf[8]<<8 | Buf[9]);
  int16_t gy=-(Buf[10]<<8 | Buf[11]);
  int16_t gz=Buf[12]<<8 | Buf[13];
  
    // Display values
  
  // Accelerometer
  Serial.print (ax,DEC); 
  Serial.print (",");
  Serial.print (ay,DEC);
  Serial.print (",");
  Serial.print (az,DEC);  
  Serial.print (",");
  
  // Gyroscope
  Serial.print (gx,DEC); 
  Serial.print (",");
  Serial.print (gy,DEC);
  Serial.print (",");
  Serial.print (gz,DEC);  
  Serial.print (",");
//Serial.println("");
  // _____________________
  // :::  Magnetometer ::: 
  // Read register Status 1 and wait for the DRDY: Data Ready
  
 /* uint8_t ST1;
//  do
//  {
    MPURead(MAG_ADDRESS,0x09,1,&ST1);
    Serial.println (ST1,BIN);
 // }
 // while (!(ST1&0x01));*/

  // Read magnetometer data  
  uint8_t Mag[7];  
  MPURead(MAG_ADDRESS,0x03,7,Mag);

  // Create 16 bits values from 8 bits data
  
  // Magnetometer
  int16_t mx=-(Mag[3]<<8 | Mag[2]);
  int16_t my=-(Mag[1]<<8 | Mag[0]);
  int16_t mz=-(Mag[5]<<8 | Mag[4]);
  
  // Magnetometer
  Serial.print (mx,DEC); 
  Serial.print (",");
  Serial.print (my-70,DEC);
  Serial.print (",");
  Serial.print (mz-700,DEC);  
  Serial.print ("\t");
  Serial.println(""); 
  // End of line
  
  delay(125); 
}
