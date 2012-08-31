#include <Interrupt.h>

#define MINT 0
#define MLED 53

Interrupt myInterrupt( MINT );

volatile bool state;

void handler(  )
{
  state = !state;
  digitalWrite( MLED, state );
}

void setup()
{
  pinMode( MLED, OUTPUT );
  digitalWrite( MLED, LOW );
  state = LOW;
  myInterrupt.SetHandler( handler );
  myInterrupt.Initialize();
}


void loop()
{
  
  
}
