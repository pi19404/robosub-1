float time = 0.0;

void setup()
{
  Serial.begin(9600);  
}

void loop()
{
  time=millis();
  Serial.println(time/1000); //print the time in seconds since the program started
  Serial.println(analogRead(0)); //print the value of A0
  Serial.println("");        //print ---- to separate values
  
  while (millis()%500!=0) //every 500 milliseconds
  {
   delay(1); //wait until it's been 500ms, 1000ms, 1500ms, etc. since the program started.
  }
   
}
