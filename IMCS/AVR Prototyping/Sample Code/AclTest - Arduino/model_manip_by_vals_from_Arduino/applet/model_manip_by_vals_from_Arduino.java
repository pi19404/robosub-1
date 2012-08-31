import processing.core.*; 
import processing.xml.*; 

import processing.opengl.*; 
import processing.serial.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class model_manip_by_vals_from_Arduino extends PApplet {




Serial sp;
byte[] buff;
float[] r;

int SIZE = 150, SIZEX = 200;
int OFFSET_X = -28, OFFSET_Y = 9;    //These offsets are chip specific, and vary.  Play with them to get the best ones for you

public void setup() {
  size(SIZEX, SIZE, P3D);
  sp = new Serial(this, "COM4",  9600);
  buff = new byte[128];
  r = new float[3];
}

float protz, protx;
public void draw() {
  //perspective( 45, 4.0/3.0, 1, 5000 );
  translate(SIZEX/2, SIZE/2, -400);
  background(0);
  buildShape(protz, protx);
  
    int bytes = sp.readBytesUntil((byte)10, buff);
    String mystr = (new String(buff, 0, bytes)).trim();
    if(mystr.split(" ").length != 3) {
      println(mystr);
      return;
    }
    setVals(r, mystr);
    
    float z = r[0], x = r[1];
    if(abs(protz - r[0]) < 0.05f)
      z = protz;
     if(abs(protx - r[1]) < 0.05f)
     x = protx;
    background(0);  
    buildShape(z, x);
    
     protz = z;     
     protx = x;
    //println(r[0] + ", " + r[1] + ", " + r[2]);
}

public void buildShape(float rotz, float rotx) {
  pushMatrix();
  scale(6,6,14);
  rotateZ(rotz);
    rotateX(rotx);
    fill(255);
     stroke(0);
     box(60, 10, 10);
     fill(0, 255, 0);
     box(10, 9, 40);
     translate(0, -10, 20);
     fill(255, 0, 0);
     box(5, 12, 10);  
  popMatrix();
}

public void setVals(float[] r, String s) {
  int i = 0;
  r[0] = -(float)(Integer.parseInt(s.substring(0, i = s.indexOf(" "))) +OFFSET_X)*HALF_PI/256;
  r[1] = -(float)(Integer.parseInt(s.substring(i+1, i = s.indexOf(" ", i+1))) + OFFSET_Y)*HALF_PI/256;
  r[2] = (float) Integer.parseInt(s.substring(i+1));
  
  
}
  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#F0F0F0", "model_manip_by_vals_from_Arduino" });
  }
}
