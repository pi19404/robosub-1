
#include <stdint.h>

int _Dir;
#define UP 0
#define DN 1

//int _Pins[] = { 70, 71, 73, 74, 76, 77, 79, 81, 83, 85 };
int _Pins[ ] = { 22, 24, 26, 28, 30, 32, 34, 36 };
#define N_PINS (sizeof(_Pins)/sizeof(int))

uint16_t _Batt;
#define BATT_MAX ((1 << N_PINS)-1)
#define BATT_MIN  0

void setup()
{
  _Batt = 0;
  _Dir = UP;
  
  for( int i = 0; i < N_PINS; ++i )
  {
    pinMode( _Pins[i], OUTPUT );
  }
}

void loop()
{
  if( UP == _Dir )
  {
     if( _Batt < BATT_MAX )
     {
         _Batt <<= 1;
         _Batt += 1;
     }
     else
     {
       _Dir = DN;
     }
  }
  else
  {
      if( _Batt > BATT_MIN )
      {
          _Batt >>= 1;
      }
      else
      {
          _Dir = UP;
      }
  }
  
  for( int i = 0; i < N_PINS; ++i )
  {
    digitalWrite( _Pins[i], (1 << i)&(_Batt) ? HIGH : LOW );
  }
  
  delay(100);
}
