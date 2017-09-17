
// Sensor graphing sketch
// This program takes ASCII-encoded strings
// from the serial port at 9600 baud and graphs them. It expects float values in the
// range of -1.0 to 1.0, followed by a newline, or newline and carriage return

// Created 20 Apr 2005
// Updated 18 Jan 2008 by Tom Igoe
// Adapted 16 Sep 2014 by Bjoern Hartmann for mbed
// This example code is in the public domain.

import processing.serial.*;
import java.util.Scanner;
import java.awt.AWTException;
import java.awt.Robot;
import java.awt.event.KeyEvent;

Serial myPort;        // The serial port
Robot robot;        // Robot object to send keyboard events

boolean q_press,e_press,r_press,w_press;
long q_time,w_time,e_time,r_time;

void setup () {
  //// set the window size:
  //size(400, 300);        

  //// List all the available serial ports to help you find the one you need
  //println(Serial.list());
  // Open whatever port is the one you're using.
  myPort = new Serial(this, "/dev/cu.usbserial-A700dZlD", 115200);
  try {
    robot = new Robot();
  } catch (AWTException e) {
    e.printStackTrace();
  }
  // don't generate a serialEvent() unless you get a newline character:
  //myPort.bufferUntil('\n');
  //// set inital background:
  //background(236,240,241);
  q_press=w_press=e_press=r_press=false;
}
void draw () {
  // everything happens in the serialEvent()
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');

  if (inString != null) {
    Scanner scanner = new Scanner(inString);
    
    // trim off any whitespace:
    inString = trim(inString);
    
    //Left
    if (inString.equals("q") && q_press==false) {
       robot.keyPress(KeyEvent.VK_CLOSE_BRACKET);
       q_press=true; 
       q_time=millis();
    }
    //Up
    else if (inString.equals("w") && w_press==false) {
      robot.keyPress(KeyEvent.VK_BACK_SLASH);
      w_press=true; 
      w_time=millis();
    } 
    //right
    else if (inString.equals("e") && e_press==false) {
      robot.keyPress(KeyEvent.VK_Q); 
      e_press=true;
      e_time=millis();
    }
    //Down
    else if (inString.equals("r") && r_press==false) {
      robot.keyPress(KeyEvent.VK_TAB);
      r_press=true;
      r_time=millis();
    }
    long time= millis();
    if(q_press && q_time-time>100){
      robot.keyRelease(KeyEvent.VK_CLOSE_BRACKET);
      q_press=false;
    }
    if(w_press && w_time-time>100){
      robot.keyRelease(KeyEvent.VK_BACK_SLASH);
      w_press=false;
    }
    if(e_press && e_time-time>100){
      robot.keyRelease(KeyEvent.VK_Q);
      e_press=false;
    }
    if(r_press && r_time-time>100){
      robot.keyRelease(KeyEvent.VK_TAB);
      r_press=false;
    }
    
    scanner.close();
  }

}