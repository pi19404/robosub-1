#include <Wire.h>

#define USE_ACCEL 1

#define ADL_DEVICE 0x1D    //ADXL345 ADL_DEVICE address
#define GYR_DEVICE 0x69    //Gyro Device Address
#define TO_READ 6        //num of bytes we are going to read each time (two bytes for each axis)

byte buff[TO_READ] ;    //6 bytes buffer for saving data read from the ADL_DEVICE
char str[512];                      //string buffer to transform data before sending it to the serial port

void setup()
{
  
 Serial.begin(9600);
 Serial1.begin(9600);

  Wire.begin();        // join i2c bus (address optional for master)
  
  //Turn on Gyros
//  writeTo(GYR_DEVICE, 0x20, 0x0F);
//  writeTo(GYR_DEVICE, 0x21, 0b00000000);
//  writeTo(GYR_DEVICE, 0x22, 0b00001000);
//  writeTo(GYR_DEVICE, 0x23, 0b00110000);
//  writeTo(GYR_DEVICE, 0x24, 0b00000000);
  
  //Turning on the ADXL345
  writeTo(ADL_DEVICE, 0x2D, 0);      
  writeTo(ADL_DEVICE, 0x2D, 16);
  writeTo(ADL_DEVICE, 0x2D, 8); 
}

void loop()
{
  int aclReadAddr = 0x32;    //first axis-acceleration-data register on the ADXL345
  //int gyrReadAddr = 0x28;
  int x, y, z;
  x = y = z = 0;
  
  readFrom(ADL_DEVICE, aclReadAddr, TO_READ, buff); //read the acceleration data from the ADXL345
  //readFrom(GYR_DEVICE, gyrReadAddr, TO_READ, buff);
 
   //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
   //thus we are converting both bytes in to one int
  x = (((int)buff[1]) << 8) | buff[0];   
  y = (((int)buff[3])<< 8) | buff[2];
  z = (((int)buff[5]) << 8) | buff[4];
  
  Serial.println( String(x) + " " + String(y) + " " + String(z) );  
  
  // command motors based on 
  sendCommand( 129, x );
  sendCommand( 130, y );
  
  delay(50);
}

// Address: 128, 129, 130
// Command: 0=m1f 1=m1b 4=m2f  5=m2b
// Data: 0 to 127
void sendCommand( uint8_t address, int16_t data )
{
  // always command 1st motor, for now (command = 0, 1)
  uint8_t command = ( data < 0 ) ? 1 : 0;
  uint8_t udata = ( data > 127 ) ? 127 : ( ( data < 0 ) ? ( ( data < -127 ) ? -127 : -data ) : data );
  Serial1.write( address ); // Address
  Serial1.write( command );
  Serial1.write( udata );
  Serial1.write( uint8_t( (address + command + udata)&(127) ) ); // Checksum
}

//---------------- Functions
//Writes val to address register on ADL_DEVICE
void writeTo(int device, byte address, byte val) {
   Wire.beginTransmission(device); //start transmission to device
   Wire.send(address);        // send register address
   Wire.send(val);        // send value to write
   Wire.endTransmission(); //end transmission
}

//reads num bytes starting from address register on ADL_DEVICE in to buff array
void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to ADL_DEVICE 
  Wire.send(address);        //sends address to read from
  Wire.endTransmission(); //end transmission
  
  Wire.beginTransmission(device); //start transmission to ADL_DEVICE
  Wire.requestFrom(device, num);    // request 6 bytes from ADL_DEVICE
  
  int i = 0;
  while(Wire.available())    //ADL_DEVICE may send less than requested (abnormal)
  { 
    buff[i] = Wire.receive(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}


