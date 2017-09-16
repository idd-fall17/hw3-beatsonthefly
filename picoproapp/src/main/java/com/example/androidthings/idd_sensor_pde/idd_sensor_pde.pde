
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

void setup () {
  //// set the window size:
  //size(400, 300);        

  //// List all the available serial ports to help you find the one you need
  //println(Serial.list());
  // Open whatever port is the one you're using.
  myPort = new Serial(this, "COM5", 9600);
  try {
    robot = new Robot();
  } catch (AWTException e) {
    e.printStackTrace();
  }
  // don't generate a serialEvent() unless you get a newline character:
  //myPort.bufferUntil('\n');
  //// set inital background:
  //background(236,240,241);
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
    if (inString.equals("q")) {
       robot.keyPress(KeyEvent.VK_LEFT);
       robot.keyRelease(KeyEvent.VK_LEFT);  
    }
    //Up
    else if (inString.equals("w")) {
      robot.keyPress(KeyEvent.VK_UP);
      robot.keyRelease(KeyEvent.VK_UP);  
    } 
    //right
    else if (inString.equals("e")) {
      robot.keyPress(KeyEvent.VK_RIGHT); 
      robot.keyRelease(KeyEvent.VK_RIGHT);  
    }
    //Down
    else if (inString.equals("r")) {
      robot.keyPress(KeyEvent.VK_DOWN);
       robot.keyRelease(KeyEvent.VK_DOWN);  
    }
    scanner.close();
  }

}