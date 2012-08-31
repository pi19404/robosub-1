#include <Wire.h>
#include "nunchuck_funcs.h"

byte zbut,cbut, joyx, joyy, incoming;

void setup()
{
    Serial.begin(9600);
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
}

void loop()
{  
      if (Serial.available() > 0) {
            incoming = Serial.read();
            if (incoming == 120){ //120 is the ascii code for the letter x
            
                  nunchuck_get_data();

                  joyx = nunchuck_joyx(); 
                  joyy = nunchuck_joyy();
                  zbut = nunchuck_zbutton();
                  cbut = nunchuck_cbutton(); 

                  Serial.write(joyx);
                  Serial.write(joyy);
                  Serial.write(zbut);
                  Serial.write(cbut);
          } 
     }
}
