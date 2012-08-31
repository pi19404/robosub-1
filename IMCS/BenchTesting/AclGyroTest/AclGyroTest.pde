#include <WireComPort.h>
#include <Wire.h>

#define NSAMPLES 50

#define GYRO   0x69
#define GCTRL1 0x20
#define GCTRL2 0x21
#define GCTRL3 0x22
#define GCTRL4 0x23
#define GCTRL5 0x24
#define GTEMP  0x26
#define GXL    0x28
#define GXH    0x29
#define GYL    0x2A
#define GYH    0x2B
#define GZL    0x2C
#define GZH    0x2D

#define ACL    0x1D
#define APCTRL 0x2D
#define AXL    0x32
#define AXH    0x33
#define AYL    0x34
#define AYH    0x35
#define AZL    0x36
#define AZH    0x37

void setupGyro( void );
void getGyroData(int16_t *x, int16_t *y, int16_t *z);
void setupAcl( void );
void getAclData( int16_t *x, int16_t *y, int16_t *z);

WireComPort Acl(Wire, ACL);
WireComPort Gyro(Wire, GYRO);
int16_t gX, gY, gZ;
int16_t gTemp;
int16_t aX, aY, aZ;
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  gX = gY = gZ = gTemp = aX = aY = aZ = 0;
  setupGyro();
  setupAcl();
  delay(1500);
}

void loop()
{
  getGyroData(&gX,&gY,&gZ);
  gTemp = Gyro.readReg(GTEMP);
  getAclData(&aX,&aY,&aZ);
  Serial.print( "Wx: ");
  Serial.print(gX);
  Serial.print("\tWy: ");
  Serial.print(gY);
  Serial.print("\tWz: ");
  Serial.print(gZ);
  Serial.print("\tTemp: ");
  Serial.print(gTemp);
  Serial.print(" degrees C.");
  Serial.print("\tX: ");
  Serial.print(aX);
  Serial.print("\tY: ");
  Serial.print(aY);
  Serial.print("\tZ: ");
  Serial.println(aZ); 
  delay(100);
}


void setupGyro( void )
{
  Gyro.writeReg(GCTRL1, 0xff); // Power on, X,Y,Z en odr 800, cutoff 110
  Gyro.writeReg(GCTRL2, 0x00); // hpf norm
  Gyro.writeReg(GCTRL3, 0x08); // DR Int En
  Gyro.writeReg(GCTRL4, 0x30); // sample rate 2000
  Gyro.writeReg(GCTRL5, 0x00); // Default fifo, int16_ts, hpf
}

void getGyroData(int16_t *x, int16_t *y, int16_t *z)
{
  int32_t xavg, yavg, zavg;
  int16_t i;
  xavg = yavg = zavg = 0;
  for( i = 0; i < NSAMPLES; ++i )
  {
   xavg += (int16_t)( (Gyro.readReg(GXH) << 8) | Gyro.readReg(GXL) );
   yavg += (int16_t)( (Gyro.readReg(GYH) << 8) | Gyro.readReg(GYL) );
   zavg += (int16_t)( (Gyro.readReg(GZH) << 8) | Gyro.readReg(GZL) );
  }
  *x = ( xavg / NSAMPLES)*70/1000;
  *y = ( yavg / NSAMPLES )*70/1000;
  *z = ( zavg / NSAMPLES )*70/1000;
}

void setupAcl( void )
{
 Acl.writeReg(APCTRL, 0x08); //Put ACL in measurement mode
}

void getAclData(int16_t *x, int16_t *y, int16_t *z)
{
  int32_t xavg, yavg, zavg;
  int16_t i;
  xavg = yavg = zavg = 0;
  for( i = 0; i < NSAMPLES; ++i )
  {
   xavg += (int32_t)( (Acl.readReg(AXH) << 8) | Acl.readReg(AXL) );
   yavg += (int32_t)( (Acl.readReg(AYH) << 8) | Acl.readReg(AYL) );
   zavg += (int32_t)( (Acl.readReg(AZH) << 8) | Acl.readReg(AZL) );
  }
  *x = ( xavg / NSAMPLES);
  *y = ( yavg / NSAMPLES );
  *z = ( zavg / NSAMPLES );
}
