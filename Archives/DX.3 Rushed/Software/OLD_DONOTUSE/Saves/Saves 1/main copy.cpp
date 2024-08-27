/**
 * Main.cpp is a testing ground for drivers and etc before
 * properly being implemented.
 * 
 *                  To Do List
 *  - HUB75 Driver          - Compatibility Manager
 *  - SD Card Protocal      - Video + Audio Decoder
 *  - Boot Startup          - HUB75 Calibration
 *  - I2C Driver
 *  - SPI Driver
 * 
 * 
 *                Progress HUB75
 *  - Binary Latch Counter      (Finished)
 *  - Row & Colomn Driver       (Finished)
 *  - Centralised Settings      (Not Started)
 *  - Bitmap to draw image      (Finished)
 *  - Image File Decoder        (Not Started)
 *  - Image Scaler              (Not Started)
 * 
 * 
 * 
*/

#include <Arduino.h>
// #include <Communications_Protocal.h>
// #include <Boards/Boards.h>
#include <ImageTest.h>
#include <HUB75.h>


//#include <Sanity_Check.h>

/**
 *      MAX7219 Driver Pinout
 *      VCC
 *      GND
 *      DIN
 *      CS
 *      CLK
 * 
*/

void setup() {
    initialise_HUB75();
    Serial.begin(9600);
}

void loop() { // 0.00117964 spf (847.7 fps); 0.00632083 spf w/ 5 buff colour (158.5 fps)

int x = 0;
for(int iiii = 0; iiii <= 75; iiii++){
    drive_HUB75(x, 5, 5, 6, 5);
}
for(int iiii = 0; iiii <= 6; iiii++){
    for(int iii = 0; iii <= 2; iii++){
        drive_HUB75(x, 5, 5, 6, 5);
    }
    x++;
}
for(int iiii = 0; iiii <= 6; iiii++){
    for(int iii = 0; iii <= 2; iii++){
        drive_HUB75(x, 5, 5, 6, 5);
    }
    x--;
}

/**
for(int iiii = 0; iiii <= 1000; iiii++){
drive_HUB75(0, 5, 5 ,6 ,5);
}
delay(500);
**/

/**
int frame = 1;
for(int iiii = 0; iiii<11; iiii++){
drive_HUB75(frame);
frame ++;
}
**/

/**
    for(int iiii = 0; iiii<1000; iiii++){
    drive_HUB75(0);
    }
**/

/**
    for(int iiii = 0; iiii<60; iiii++){drive_HUB75(1);}
    for(int iiii = 0; iiii<60; iiii++){drive_HUB75(2);}
    for(int iiii = 0; iiii<60; iiii++){drive_HUB75(3);}
    for(int iiii = 0; iiii<60; iiii++){drive_HUB75(4);}
    for(int iiii = 0; iiii<60; iiii++){drive_HUB75(5);}
    for(int iiii = 0; iiii<60; iiii++){drive_HUB75(6);}
    for(int iiii = 0; iiii<60; iiii++){drive_HUB75(7);}

    for(int iiii = 0; iiii<61; iiii++){drive_HUB75(7);}
    for(int iiii = 0; iiii<61; iiii++){drive_HUB75(6);}
    for(int iiii = 0; iiii<61; iiii++){drive_HUB75(5);}
    for(int iiii = 0; iiii<61; iiii++){drive_HUB75(4);}
    for(int iiii = 0; iiii<61; iiii++){drive_HUB75(3);}
    for(int iiii = 0; iiii<61; iiii++){drive_HUB75(2);}
    for(int iiii = 0; iiii<61; iiii++){drive_HUB75(1);}
**/
}