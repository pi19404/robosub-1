#include <ArduinoComMessage.h>
#include <Adxl345_Data.h>
#include <Wire.h>

#define ACL_PORT 0x1D

Adxl345_Data      gblAclData;
ArduinoComMessage gblPcMsg;

void SendDataToPC( void );
void GetDataFromPC( void );
void SetAclConfig( void );
void GetAclConfig( void );

void setup()
{
  Serial.begin(9600);
  Wire.begin(ACL_PORT);
  
}

void loop()
{
  
}

void serialEvent()
{
  GetDataFromPC();
  SetAclConfig();
  GetAclConfig();
  SendDataToPC();
}

void SendDataToPC( void )
{
  ArduinoComMessage msg;
  msg.SetSystem("MCU");
  msg.SetSubSystem("Acl");
  msg.SetMessageType("Status");
  String msgData = ArduinoComMessage::DataToString<Adxl345_Data>( gblAclData );
  msg.SetMessageData( msgData );
  
  String msgString = msg.ToString();
  Serial.print( msgString );
}

void GetDataFromPC( void )
{
  String comStr;
  while( Serial.available() )
  {
    comStr += Serial.read();
  }
  gblPcMsg = ArduinoComMessage( comStr );
  gblAclData = ArduinoComMessage::StringToData<Adxl345_Data>( gblPcMsg.GetMessageData() );
}

void SetAclConfig( void )
{
  char* aclUData = reinterpret_cast<char *>( &gblAclData );
  
  // Write 1st Batch
  Wire.beginTransmission( ACL_PORT );
  Wire.write(Adxl345_Registers[eTapThreshold].mAddress);
  for( int i = eTapThreshold; i != eTapSource; ++i )
  {
    Wire.write( aclUData[i] );
  }
  Wire.endTransmission();
  
  // Write 2nd Batch
  Wire.beginTransmission( ACL_PORT );
  Wire.write(Adxl345_Registers[eDataRatePowerModeControl].mAddress);
  for( int i = eDataRatePowerModeControl; i != eInterruptSource; ++i )
  {
    Wire.write(aclUData[i]);
  }
  Wire.endTransmission();
  
  // Write 3rd Batch
  Wire.beginTransmission( ACL_PORT );
  Wire.write(Adxl345_Registers[eDataFormatControl].mAddress);
  Wire.write(aclUData[eDataFormatControl]);
  Wire.endTransmission();
  
  // Write 4th Batch
  Wire.beginTransmission( ACL_PORT );
  Wire.write(Adxl345_Registers[eFifoControl].mAddress);
  Wire.write(aclUData[eFifoControl]);
  Wire.endTransmission();
}
  
void GetAclConfig( void )
{
  char* aclUData = reinterpret_cast<char *>( &gblAclData );
  Wire.beginTransmission( ACL_PORT );
  Wire.write(Adxl345_Registers[eDeviceId].mAddress);
  Wire.endTransmission();
  Wire.requestFrom( Adxl345_Registers[eDeviceId].mAddress, sizeof(gblAclData) );
  
  int i = 0;
  while( Wire.available() )
  {
      aclUData[i] = Wire.read();
      ++i;
  }
}

