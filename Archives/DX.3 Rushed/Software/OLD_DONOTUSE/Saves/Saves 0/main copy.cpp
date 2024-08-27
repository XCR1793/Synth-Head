/**
 * Main.cpp is a testing ground for drivers and etc before
 * properly being implemented.
 * 
 *                  To Do List
 *  - HUB75 Driver          - Compatibility Manager
 *  - SD Card Protocal      - Video + Audio Decoder
 *  - Boot Startup          - HUB75 Calibration
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
 * HUB75 Driver Testing in progress
 *      Matrix circuit schematic
 *  - https://www.crowdsupply.com/img/73d3/led-panel-dchy-p2-644-1515-vp-sch.jpg
 * 
 *      Matrix column driver datasheet
 *  - https://olympianled.com/wp-content/uploads/2021/05/ICN2037_datasheet_EN_2017_V2.0.pdf
 * 
 *      Matrix row driver datasheet
 *  - https://datasheetspdf.com/pdf-file/1348504/FUMAN/TC7258/1
 *  - https://datasheetspdf.com/pdf/1328895/DBIC/D7258/1
 * 
 * 
*/

#include <Arduino.h>
// #include <Communications_Protocal.h>
// #include <Boards/Boards.h>
#include <ImageTest.h>



/**
 *      HUB75 Pinout
 *      R0  G0
 *      B0  GND
 *      R1  G1
 *      B1  GND/E
 *      A   B
 *      C   D
 *      CLK LAT
 *      OE  GND
 * 
*/

//#include <Sanity_Check.h>


#define HUBR0   32
#define HUBB0   23
#define HUBR1   25
#define HUBB1   26
#define HUBA    27
#define HUBC    14
#define HUBCLK  12
#define HUBOE   13

#define HUBG0   21
#define HUBG1   19
#define HUBE    18
#define HUBB    5
#define HUBD    17
#define HUBLAT  16

void setup() {

    pinMode(HUBR0, OUTPUT);
    pinMode(HUBB0, OUTPUT);
    pinMode(HUBR1, OUTPUT);
    pinMode(HUBB1, OUTPUT);
    pinMode(HUBA, OUTPUT);
    pinMode(HUBC, OUTPUT);
    pinMode(HUBCLK, OUTPUT);
    pinMode(HUBOE, OUTPUT);
    pinMode(HUBG0, OUTPUT);
    pinMode(HUBG1, OUTPUT);
    pinMode(HUBE, OUTPUT);
    pinMode(HUBB, OUTPUT);
    pinMode(HUBD, OUTPUT);
    pinMode(HUBLAT, OUTPUT);

    Serial.begin(9600);
}

void loop() {

    int x = 0;     // Run speed
    int latcount = 0;
    int cycle = 0;
    int cycle2 = 1024;
    digitalWrite(HUBLAT, LOW);
    digitalWrite(HUBOE, LOW);
    digitalWrite(HUBCLK, HIGH);
    for(int i = 0; i <= 15; i++){
        bool latcheck = true;
        if((latcheck = latcount & 0x01)){digitalWrite(HUBA, HIGH);/*Serial.print("A");*/} else {digitalWrite(HUBA, LOW);}
        if((latcheck = latcount & 0x02)){digitalWrite(HUBB, HIGH);/*Serial.print("B");*/} else {digitalWrite(HUBB, LOW);}
        if((latcheck = latcount & 0x04)){digitalWrite(HUBC, HIGH);/*Serial.print("C");*/} else {digitalWrite(HUBC, LOW);}
        if((latcheck = latcount & 0x08)){digitalWrite(HUBD, HIGH);/*Serial.print("D");*/} else {digitalWrite(HUBD, LOW);}
        if((latcheck = latcount & 0x10)){digitalWrite(HUBE, HIGH);/*Serial.print("E");*/} else {digitalWrite(HUBE, LOW);}
        latcount = latcount + 1;
        // Serial.println(" ");


    for(int i = 0; i <= 63; i++) {
        bool ledseq = true;
        if((ledseq = testimage[cycle ] & 0x01)){digitalWrite(HUBR0, HIGH)/*Serial.print(" R0")*/;} else {digitalWrite(HUBR0, LOW);}
        if((ledseq = testimage[cycle ] & 0x02)){digitalWrite(HUBG0, HIGH)/*Serial.print(" G0")*/;} else {digitalWrite(HUBG0, LOW);}
        if((ledseq = testimage[cycle ] & 0x04)){digitalWrite(HUBB0, HIGH)/*Serial.print(" B0")*/;} else {digitalWrite(HUBB0, LOW);}
        if((ledseq = testimage[cycle2] & 0x01)){digitalWrite(HUBR1, HIGH)/*Serial.print(" R1")*/;} else {digitalWrite(HUBR1, LOW);}
        if((ledseq = testimage[cycle2] & 0x02)){digitalWrite(HUBG1, HIGH)/*Serial.print(" G1")*/;} else {digitalWrite(HUBG1, LOW);}
        if((ledseq = testimage[cycle2] & 0x04)){digitalWrite(HUBB1, HIGH)/*Serial.print(" B1")*/;} else {digitalWrite(HUBB1, LOW);}

    digitalWrite(HUBCLK, LOW);
    delayMicroseconds(x);
    digitalWrite(HUBCLK, HIGH);
    delayMicroseconds(x);
    cycle = cycle + 1;
    cycle2 = cycle2 + 1;
    // Serial.println("-----");
    // Serial.println(cycle);
    }
    
    delayMicroseconds(x);
    digitalWrite(HUBLAT, HIGH);
    delayMicroseconds(x);
    digitalWrite(HUBLAT, LOW);
    delayMicroseconds(x);

    digitalWrite(HUBOE, HIGH);
    delayMicroseconds(x);
    digitalWrite(HUBOE, LOW);
    delayMicroseconds(x);
    // Serial.println("ROW done -----");
    }

    delay(0);
    // Serial.println("Ran Once");
}