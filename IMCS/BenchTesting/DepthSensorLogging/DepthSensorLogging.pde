#define D_PIN A7

int i;
long prev;
void setup()
{
  Serial.begin(9600);
  pinMode(D_PIN, INPUT );
  i = prev = 0;
}

void loop()
{
  prev = millis();
  Serial.println( analogRead(D_PIN ) );
  
  int time = millis();
  if( (time - prev) < 1000 )
  {
     delay( 1000 - (time - prev ) );
  }
}
