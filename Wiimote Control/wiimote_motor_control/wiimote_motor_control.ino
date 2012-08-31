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

const int ctrl1 = 128;
const int ctrl2 = 129;
const int ctrl3 = 130;
const int m1_ctrl = ctrl1;
const int m2_ctrl = ctrl2;
const int m3_ctrl = ctrl1;
const int m4_ctrl = ctrl2;
const int max_volts = int( 22*5.12 ); // value is voltage * 5.12
const int max_volt_cmd = 3;

int m1_dir=0; //default in the forward direction
int m2_dir=0; //default in the forward direction
int m1_speed=0;
int m2_speed=0;

float speed_scale_factor=0.70;

int m3_dir=4; //default in the forward direction
int m4_dir=4; //default in the forward direction
int vertical_speed=0;
int c_state=0;
int z_state=0;
int button_press=0;

int debug=1;
int z,c,x,y;

void setup()
{
    if (debug==1)
      Serial.begin(9600);  //programming & debug serial port
    
    Serial1.begin(9600); //motor controller serial port
    
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
    
    delay(2000);
    
    // Initialize all motors to 0
    sendCommand(m1_ctrl,m1_dir,m1_speed);
    sendCommand(m2_ctrl,m2_dir,m2_speed);
    sendCommand(m3_ctrl,m3_dir,vertical_speed);
    sendCommand(m4_ctrl,m4_dir,vertical_speed);
    
    // Initialize max voltages of all three controllers
    sendCommand(ctrl1,max_volt_cmd,max_volts);
    sendCommand(ctrl2,max_volt_cmd,max_volts);
    sendCommand(ctrl3,max_volt_cmd,max_volts);
}

void loop()
{
        nunchuck_get_data();
        
        x = nunchuck_joyx()-128; // offset the joystick's default position (it's not quite centered)
        y = nunchuck_joyy()-127; // offset the joystick's default position (it's not quite centered)
        z = nunchuck_zbutton();
        c = nunchuck_cbutton(); 
       
        if (debug==1)
        {
          Serial.print("Joystick: ("); Serial.print(x); Serial.print(","); 
          Serial.print(y); Serial.println(")");
          //Serial.print(" C: ");Serial.print(c);
          //Serial.print(" Z: "); Serial.println(z);
        }
  
   //****// Control of Motors 1 and 2 //****//
   
       //All Cases:
       m1_speed = (-y)-x;
       m2_speed = (-y)+x;
       
       if( m1_speed > 127 )
       {
         m1_speed = 127;
       }
       else if( m1_speed < -127 )
       {
         m1_speed = -127;
       }
       
       if( m2_speed > 127 )
       {
         m2_speed = 127;
       }
       else if( m2_speed < -127 )
       {
         m2_speed = -127;
       }
       
       
       // Determine direction
       if( m1_speed < 0 )
       {
         m1_dir = 1;
         m1_speed = (-m1_speed);
       }
       else
       {
         m1_dir = 0;
       }
       
       if( m2_speed < 0 )
       {
          m2_dir = 1;
          m2_speed = (-m2_speed);
       }
       else
       {
         m2_dir = 0;
       }
       
       //send commands to motor controllers
       sendCommand(m1_ctrl, m1_dir, int(m1_speed * speed_scale_factor));
       sendCommand(m2_ctrl, m2_dir, int(m2_speed * speed_scale_factor));
       
   //****// - END - Control of Motors 1 and 2 //****//
        
   //****// Control of Motors 3 and 4 //****//   
        if (c != c_state) {              // the button state has changed
          if (c == LOW) {                // check if the button is pressed
            vertical_speed+=10;          // increment speed
            if (debug==1)
            {
              Serial.print("Vertical Speed:");
              Serial.println(vertical_speed);
            }
            button_press=1;
          }
        }
        c_state = c;                      // save the new state   
      
        if (z != z_state) {              // the button state has changed
          if (z == LOW) {                // check if the button is pressed
            vertical_speed-=10;          // decrement speed
            if (debug==1)
            {
              Serial.print("Vertical Speed:");
              Serial.println(vertical_speed);
            }
            button_press=1;
          }
        }
        z_state = z;                      // save the new state  
        
        if (button_press==1)
        {
            if (vertical_speed < 0)
            {
              m3_dir=4; //down, forward motor
              m4_dir=4; 
            }
            else
            {
              m3_dir=5; //up, backward motor
              m4_dir=5;
            }
            
            sendCommand(m3_ctrl, m3_dir, int(abs(vertical_speed) * speed_scale_factor));
            sendCommand(m4_ctrl, m4_dir, int(abs(vertical_speed) * speed_scale_factor));
            button_press=0;
        }
        
   //****// - END - Control of Motors 3 and 4 //****//  
        
        Serial.print("m1 dir:");Serial.print(m1_dir);
        Serial.print("  m2 dir:");Serial.print(m2_dir);
        Serial.print("  m3 dir:");Serial.print(m3_dir);
        Serial.print("  m4 dir:");Serial.println(m4_dir);
        
        delay(50);
}

/* 
   Addresses of motor controllers: 
   128 - Motor 1 = 1, Motor 2 = 3
   129 - Motor 1 = 2, Motor 2 = 4
   130 - Motor 1 = 5, Motor 2 = 6 (Eventually) 
     
       Front
      ____3____
     |         |
     |  Sub    |
   1 | (Plan   | 2
     |  View)  |
     |________ |
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
