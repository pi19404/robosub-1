#include <Wire.h>
#include <SPI.h>
#include <WireComPort.h>
#include <stdint.h>

#define COMPASS_ADDR 0x19  // Compass Wire Address
#define CMP_HPR_REG  0x50  // Compass Heading, Pitch and Roll Register
#define LOOP_RATE_mHz 500  // Set delay in milliseconds

//Initialize the WireComPort for the Compass
WireComPort CompassCom( Wire, COMPASS_ADDR );

// Compass Data:
int16_t Heading, Pitch, Roll;
float Hflt, Pflt, Rflt;
// delay info
unsigned long PrevClock;
void setup()
{
  // Initialize globals:
  Heading = Pitch = Roll = 0;
  Hflt = Pflt = Rflt = 0.0;
  
  // Begin serial com with pc
  Serial.begin(9600);
  
  // Join the I2C Bus as master
  Wire.begin();
  
  // Wait 500ms for compass to initialize
  delay(500);
}


void loop()
{
  // Get Compass Heading, Pitch, and Roll
  PrevClock = millis();
  uint8_t buffer[6], *ptr;
  CompassCom.readReg(CMP_HPR_REG, 6, buffer);
  
  // Update Heading, Pitch, and Roll, byte order is LOW-> MSB LSB ->HIGH
  ptr = (uint8_t *)&Heading;
  ptr[1] = buffer[0]; ptr[0] = buffer[1];
  ptr = (uint8_t *)&Pitch;
  ptr[1] = buffer[2]; ptr[0] = buffer[3];
  ptr = (uint8_t *)&Roll;
  ptr[1] = buffer[4]; ptr[0] = buffer[5];
  
  // Heading, Pitch and Roll are in tenths of a degree
  // Print their floating point equivalents (in Degrees)
  Hflt = (float)Heading / 10.0;
  Pflt = (float)Pitch   / 10.0;
  Rflt = (float)Roll    / 10.0;
  //Serial.print("Heading: ");
  Serial.print( Hflt );
  //Serial.print("\tPitch: ");
  Serial.print("\t");
  Serial.print( Pflt );
  //Serial.print("\tRoll: ");
  Serial.print("\t");
  Serial.println( Rflt );
  
  // Delay 0.5 seconds per loop so that
  // the output remains human readable.
  while( abs(millis() - PrevClock) < LOOP_RATE_mHz );
}
