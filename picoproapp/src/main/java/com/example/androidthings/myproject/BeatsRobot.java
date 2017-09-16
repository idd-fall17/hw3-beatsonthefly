package com.example.androidthings.myproject;


import android.app.Instrumentation;

/**
 * Created by KiANMiNG on 16/9/2017.
 */

public class BeatsRobot {
    Instrumentation inst = new Instrumentation();
    public BeatsRobot () {
    }

//    Reads in an integer value corresponding to a direction
//    1 - Left
//    2 - Up
//    3 - Right
//    4 - Down
    public void direction(int dir) {
        switch (dir) {
            case 1: {
                inst.sendKeyDownUpSync(37);
            }
            case 2: {
                inst.sendKeyDownUpSync(38);
            }
            case 3: {
                inst.sendKeyDownUpSync(39);
            }
            case 4: {
                inst.sendKeyDownUpSync(40);
            }
        }
    }
}
