

void imHigh()
{
  if( digitalRead(2) )
  {
     Serial.println("I'm High Now");
  }
  else
  {
     Serial.println("I'm Low Now" );
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  attachInterrupt(0, imHigh, CHANGE);  
}

void loop()
{
  
}
