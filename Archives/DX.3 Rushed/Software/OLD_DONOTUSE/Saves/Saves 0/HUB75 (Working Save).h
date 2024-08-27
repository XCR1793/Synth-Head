#pragma once
/**
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
*/

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

void initialise_HUB75() {
    #define HUBR0   19
    #define HUBB0   18
    #define HUBR1   5
    #define HUBB1   17
    #define HUBA    16
    #define HUBC    4
    #define HUBCLK  0
    #define HUBOE   2

    #define HUBG0   25
    #define HUBG1   26
    #define HUBE    27
    #define HUBB    14
    #define HUBD    12
    #define HUBLAT  13

    #define LED_Matrix_Height 32
    #define LED_Matrix_Length 64
    #define LED_Pixel_Count LED_Matrix_Height * LED_Matrix_Length
    #define LED_Matrix_Brightness 255

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
}


void drive_HUB75(int Fnum) {

    // , int Buffcount, int Rvalues, int Gvalues, int Bvalues
    // int x = 0;     // Run speed
    int latcount = 0;
    int scan = 16;
    int cycle = 0;
    int cycle2 = LED_Pixel_Count/2;

    //int RColourSub = 1;
    //int GColourSub = 1;
    //int BColourSub = 1;
//
    //int CurrentBuff = 0;
//
    //for(int i = 0; i < Rvalues; i++){RColourSub = RColourSub * 2;}; RColourSub = RColourSub/Buffcount;
    //for(int i = 0; i < Gvalues; i++){GColourSub = GColourSub * 2;}; GColourSub = GColourSub/Buffcount;
    //for(int i = 0; i < Bvalues; i++){BColourSub = BColourSub * 2;}; BColourSub = BColourSub/Buffcount;
//
//
    //int RBitMask = 0;
    //int GBitMask = 0;
    //int BBitMask = 0;
    //for(int i = 0; i < Rvalues; i++){RBitMask = RBitMask << 1; RBitMask = RBitMask | 0b1;} RBitMask = RBitMask << (Gvalues + Bvalues);
    //for(int i = 0; i < Gvalues; i++){GBitMask = GBitMask << 1; GBitMask = GBitMask | 0b1;} GBitMask = GBitMask << (Bvalues);
    //for(int i = 0; i < Bvalues; i++){BBitMask = BBitMask << 1; BBitMask = BBitMask | 0b1;}



// for(int i = 1; i <= Buffcount; i++){

    for(int ii = 1; ii <= scan; ii++){
    digitalWrite(HUBLAT, HIGH);
    // delayMicroseconds(x);
    digitalWrite(HUBLAT, LOW);
    // delayMicroseconds(x);
    digitalWrite(HUBLAT, HIGH);
    // delayMicroseconds(x);

    digitalWrite(HUBOE, HIGH);
    // delayMicroseconds(x);
    digitalWrite(HUBOE, LOW);
    // delayMicroseconds(x);
    

    for(int ii = 1; ii <= LED_Matrix_Length; ii++) {

        

        // if((1 <= ((frame[Fnum][cycle] & RBitMask)>>(Gvalues + Bvalues))-RColourSub*CurrentBuff)){digitalWrite(HUBR0,HIGH);/*Serial.print(" R0");*/} else {digitalWrite(HUBR0,LOW);}
        // if((1 <= ())){digitalWrite(HUBG0,HIGH);/*Serial.print(" G0");*/} else {digitalWrite(HUBG0,LOW);}
        // if((1 <= ())){digitalWrite(HUBB0,HIGH);/*Serial.print(" B0");*/} else {digitalWrite(HUBB0,LOW);}
        // if((1 <= ())){digitalWrite(HUBR1,HIGH);/*Serial.print(" R1");*/} else {digitalWrite(HUBR1,LOW);}
        // if((1 <= ())){digitalWrite(HUBG1,HIGH);/*Serial.print(" G1");*/} else {digitalWrite(HUBG1,LOW);}
        // if((1 <= ())){digitalWrite(HUBB1,HIGH);/*Serial.print(" B1");*/} else {digitalWrite(HUBB1,LOW);}

        // Serial.print((((frame[Fnum][cycle ] & RBitMask) >> (Gvalues + Bvalues))));
        // Serial.print(RColourSub);
        // Serial.print(" ");

        bool ledseq = true;
        if((ledseq = frame[Fnum][cycle ] & 0x01)){digitalWrite(HUBR0, HIGH);/*Serial.print(" R0")*/} else {digitalWrite(HUBR0, LOW);}
        if((ledseq = frame[Fnum][cycle ] & 0x02)){digitalWrite(HUBG0, HIGH);/*Serial.print(" G0")*/} else {digitalWrite(HUBG0, LOW);}
        if((ledseq = frame[Fnum][cycle ] & 0x04)){digitalWrite(HUBB0, HIGH);/*Serial.print(" B0")*/} else {digitalWrite(HUBB0, LOW);}
        if((ledseq = frame[Fnum][cycle2] & 0x01)){digitalWrite(HUBR1, HIGH);/*Serial.print(" R1")*/} else {digitalWrite(HUBR1, LOW);}
        if((ledseq = frame[Fnum][cycle2] & 0x02)){digitalWrite(HUBG1, HIGH);/*Serial.print(" G1")*/} else {digitalWrite(HUBG1, LOW);}
        if((ledseq = frame[Fnum][cycle2] & 0x04)){digitalWrite(HUBB1, HIGH);/*Serial.print(" B1")*/} else {digitalWrite(HUBB1, LOW);}
       

    digitalWrite(HUBCLK, LOW);
    // delayMicroseconds(x);
    digitalWrite(HUBCLK, HIGH);
    // delayMicroseconds(x);
    cycle = cycle + 1;
    cycle2 = cycle2 + 1;
    // Serial.println("-----");
    // Serial.println(cycle);

    }

    digitalWrite(HUBOE, HIGH);
    // delayMicroseconds(x);

    bool latcheck = true;
        if((latcheck = latcount & 0x01)){digitalWrite(HUBA, HIGH);/*Serial.print("A");*/} else {digitalWrite(HUBA, LOW);}
        if((latcheck = latcount & 0x02)){digitalWrite(HUBB, HIGH);/*Serial.print("B");*/} else {digitalWrite(HUBB, LOW);}
        if((latcheck = latcount & 0x04)){digitalWrite(HUBC, HIGH);/*Serial.print("C");*/} else {digitalWrite(HUBC, LOW);}
        if((latcheck = latcount & 0x08)){digitalWrite(HUBD, HIGH);/*Serial.print("D");*/} else {digitalWrite(HUBD, LOW);}
        if((latcheck = latcount & 0x10)){digitalWrite(HUBE, HIGH);/*Serial.print("E");*/} else {digitalWrite(HUBE, LOW);}

    // Serial.println("ROW done -----");

    latcount = latcount + 1;
    }
    
    // Serial.println(RColourSub);
    // Serial.println(RColourThresh);

    // CurrentBuff ++;

    // Serial.println("Ran Once");
    // delay(500);
// }
}
