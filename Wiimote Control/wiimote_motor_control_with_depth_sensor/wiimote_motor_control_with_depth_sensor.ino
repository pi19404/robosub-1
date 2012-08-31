/*

Authors: Joe Langevin, Ben Brand, Jay Sidhu, various people on the interwebs

This program controls 4 thrusters 
The thrusters are connected to Sabertooth 2x12 motor controllers
Thruster direction and speed is controlled by a WiiMote joystick

Written for the Arduino Mega

Wiimote needs to be connected to I2C:
  - Wiimote "D" needs to be connected to pin SDA (pin 20)
  - Wiimote "C" needs to be connected to pin SCL (pin 21)
  - GND for wiimote needs to be connected to GND
  - PWR for wiimote needs to be connected to 5v  
  
Serial Port 0 = Debug & Programming
Serial Port 1 = Motor Controllers

*/

#include <Wire.h>
#include "nunchuck_funcs.h"

int m1_dir=0; //default in the forward direction
int m2_dir=0; //default in the forward direction
int m1_speed=0;
int m2_speed=0;

int m3_dir=4; //default in the forward direction
int m4_dir=4; //default in the forward direction
int vertical_speed=0;
int c_state=0;
int z_state=0;

int current_depth=A0; //TODO: what is the range of this input??
int desired_depth=0;  //TODO: what depth should this be initialized to?

int debug=1;
int z,c,x,y;

void setup()
{
    if (debug==1)
      Serial.begin(9600);  //programming & debug serial port
    
    Serial1.begin(9600); //motor controller serial port
    
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
}

void loop()
{
        nunchuck_get_data();
        
        x = nunchuck_joyx()-128; // offset the joystick's default position (it's not quite centered)
        y = nunchuck_joyy()-126; // offset the joystick's default position (it's not quite centered)
        z = nunchuck_zbutton();
        c = nunchuck_cbutton(); 
       
        if (debug==1)
        {
          Serial.print("Joystick: ("); Serial.print(x); Serial.print(","); 
          Serial.print(y); Serial.print(")");
          //Serial.print(" C: ");Serial.print(c);
          //Serial.print(" Z: "); Serial.println(z);
        }
  
   //****// Control of Motors 1 and 2 //****//
   
       //determine the speed of each motor based upon x,y joystick coordinates
       if (x>=0 && y>=0) //turn right while going forward
       {
         m1_speed = (int) (y + x)/2;
         m2_speed = (int) (y + (-x) )/2;
       }
       else if (x<0 && y>0) //turn left while going forward
       {
         m1_speed = (int) (y + (-x) )/2;
         m2_speed = (int) (y + x)/2;
       }
       else if (x>0 && y<0) //turn right while going backward
       {
         m1_speed = (int) ( (-y) + x)/2;
         m2_speed = (int) ( (-y) + (-x) )/2;
       }
       else //if (x<0 && y<0) //turn left while going backward
       {
         m1_speed = (int) ( (-y) + (-x) )/2;
         m2_speed = (int) ( (-y) +  x)/2;
       }
       
       //set direction of motors
       if (m1_speed < 0)
          m1_dir=1;
        else
          m1_dir=0;
        
        if (m2_speed <0)
          m2_dir=1;
        else
          m2_dir=0;
    
       //send commands to motor controllers
       sendCommand(128, m1_dir, abs(m1_speed));
       sendCommand(129, m2_dir, abs(m2_speed));
       
   //****// - END - Control of Motors 1 and 2 //****//
        
   //****// Control of Motors 3 and 4 //****//   
          
        if (c != c_state) {              // the button state has changed
          if (c == LOW) {                // check if the button is pressed
            desired_depth+=5;            // increment depth
            if (debug==1)
            {
              Serial.print("Desired Depth:");
              Serial.println(desired_depth);
            }
          }
        }
        c_state = c;                      // save the new state   
      
        if (z != z_state) {              // the button state has changed
          if (z == LOW) {                // check if the button is pressed
            desired_depth-=5;            // decrement depth
            if (debug==1)
            {
              Serial.print("Desired Depth:");
              Serial.println(desired_depth);
            }
          }
        }
        z_state = z;                      // save the new state  
        
        // Auto-Adjust depth based upon the depth sensor and desired depth
        if (current_depth >= desired_depth)
        {
           m3_dir=5; //backward
           m4_dir=5;
        }
        else 
        {
           m3_dir=4; //forward
           m4_dir=4;
        }
        
        vertical_speed = (int) 5*abs(current_depth-desired_depth); // the multiplier of 5 is just a guess for now.
       
        if (debug == 1)
        {
          Serial.print("Current Depth:");
          Serial.println(current_depth);
        }
       
        sendCommand(128, m3_dir, vertical_speed);
        sendCommand(129, m4_dir, vertical_speed);
        
   //****// - END - Control of Motors 3 and 4 //****//  
        
        delay(50);
}

/* 
   Addresses of motor controllers: 
   128 - Motor 1 = 1, Motor 2 = 3
   129 - Motor 1 = 2, Motor 2 = 4
   130 - Motor 1 = 5, Motor 2 = 6 (Eventually) 
     
      ___3___
     |       |
     |  Sub  |
   1 | (Top  | 2
     | View) |
     |_______|
         4

 Direction Commands: 
    0=Motor1Forward  
    1=Motor1Backward
    4=Motor2Forward
    5=Motor2Backward
    
 Speed: 0 to 127
 
*/

void sendCommand( uint8_t address, uint8_t m_dir, int16_t m_speed )
  {
      uint8_t command = m_dir;
      uint8_t udata = m_speed;
      Serial1.write( address ); 
      Serial1.write( m_dir );
      Serial1.write( m_speed );
      Serial1.write( uint8_t( (address + m_dir + m_speed)&(127) ) ); // Checksum
}
