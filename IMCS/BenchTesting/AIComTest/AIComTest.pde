
#include <stdint.h>

#define NUM_VALS 5
#define DELAY_MS 10

int8_t sendData[NUM_VALS];
int8_t recvData[NUM_VALS];

int ledPin = 12;

void flashLed( int nFlash )
{
  for( int i = 0; i < nFlash; ++i ) {
    digitalWrite(ledPin, HIGH );
    delay(250);
    digitalWrite(ledPin, LOW );
    delay(250);
  }
}
  

void setup()
{
  Serial.begin(115200);
  for( int i = 0; i < NUM_VALS; ++i ) {
    sendData[i] = recvData[i] = i+'0';
  }
  pinMode(ledPin, OUTPUT);  
  //delay(500);
  
 // Serial.write("^^$@");
  
  //flashLed( 5 );
}

void loop()
{
  
  // get clock time
  int nWrong = 0;
  long ctime, ptime;
  ptime = millis();
  
  Serial.write( (uint8_t *)sendData, NUM_VALS );
  //Serial.flush();
  
  while( Serial.available() < NUM_VALS ) {
    delay(1);
  }
 
  for( int i = 0; i < NUM_VALS; ++i )
  {
      recvData[i] = Serial.read();
  }
  
  for( int i = 0; i < NUM_VALS; ++i )
  {
     if( recvData[i] != i+'0' )
     {
       ++nWrong;
     }
  }
  
//  flashLed( nWrong );
  
  
  // verify loop rate
  ctime = millis();
  ctime -= ptime;
  if( ctime < DELAY_MS )
  {
    delay( DELAY_MS - ctime );
  }
}
