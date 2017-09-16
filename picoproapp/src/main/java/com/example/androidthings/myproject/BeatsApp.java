package com.example.androidthings.myproject;

import android.util.Log;
import com.google.android.things.pio.Gpio;

import java.io.IOException;

import com.google.android.things.contrib.driver.mma8451q.Mma8451Q;

/**
 * HW3 Template
 * Created by bjoern on 9/12/17.
 * Wiring:
 * USB-Serial Cable:
 *   GND to GND on IDD Hat
 *   Orange (Tx) to UART6 RXD on IDD Hat
 *   Yellow (Rx) to UART6 TXD on IDD Hat
 * Accelerometer:
 *   Vin to 3V3 on IDD Hat
 *   GND to GND on IDD Hat
 *   SCL to SCL on IDD Hat
 *   SDA to SDA on IDD Hat
 * Analog sensors:
 *   Middle of voltage divider to Analog A0..A3 on IDD Hat
 */

public class BeatsApp extends SimplePicoPro {

//    Mma8451Q accelerometer;
//
//    float[] xyz = {0.f,0.f,0.f}; //store X,Y,Z acceleration of MMA8451 accelerometer here [units: G]
//    float a0,a1,a2,a3; //store analog readings from ADS1015 ADC here [units: V]

    public void setup() {

        // Initialize the serial port for communicating to a PC
        uartInit(UART6,9600);
//
//        // Initialize the Analog-to-Digital converter on the HAT
//        analogInit(); //need to call this first before calling analogRead()
//
//        // Initialize the MMQ8451 Accelerometer
//        try {
//            accelerometer = new Mma8451Q("I2C1");
//            accelerometer.setMode(Mma8451Q.MODE_ACTIVE);
//        } catch (IOException e) {
//            Log.e("HW3Template","setup",e);
//        }
        //dot button
        pinMode(GPIO_128, Gpio.DIRECTION_IN);
        setEdgeTrigger(GPIO_128,Gpio.EDGE_BOTH);

        //dash button
        pinMode(GPIO_39, Gpio.DIRECTION_IN);
        setEdgeTrigger(GPIO_39,Gpio.EDGE_BOTH);

        //backspace button
        pinMode(GPIO_37, Gpio.DIRECTION_IN);
        setEdgeTrigger(GPIO_37,Gpio.EDGE_BOTH);

        //enter button
        pinMode(GPIO_35, Gpio.DIRECTION_IN);
        setEdgeTrigger(GPIO_35,Gpio.EDGE_BOTH);
    }

    public void loop() {
//        // read I2C accelerometer and print to UART
//        try {
//            println(UART
//
//            //use this line instead for unlabeled numbers separated by tabs that work with Arduino's SerialPlotter:
//            //println(UART6,xyz[0]+"\t"+xyz[1]+"\t"+xyz[2]); // this goes to the Serial port
//
//        } catch (IOException e) {
//            Log.e("BeatsApp","loop",e);
//        }
//
//        delay(100);
    }

    public void leftArrowKeyPress () {
        printStringToScreen("LEFT ");
        println(UART6,"q");
    }

    public void upArrowKeyPress () {
        printStringToScreen("UP ");
        println(UART6,"w");
    }

    public void rightArrowKeyPress () {
        printStringToScreen("RIGHT ");
        println(UART6,"e");
    }

    public void downArrowKeyPress () {
        printStringToScreen("DOWN ");
        println(UART6,"r");
    }

    void digitalEdgeEvent(Gpio pin, boolean value) {

        //Q
        if(pin==GPIO_128 && value==HIGH) {
            printStringToScreen("Q");
            println(UART6,"q");
        }

        //W
        else if(pin==GPIO_39 && value==HIGH) {
            printStringToScreen("W");
            println(UART6,"w");
        }

        //E
        else if(pin==GPIO_37 && value==HIGH) {
            printStringToScreen("E");
            println(UART6,"e");
        }

        //R
        else if(pin==GPIO_35 && value==HIGH) {
            printStringToScreen("R");
            println(UART6,"r");
        }
    }
}
