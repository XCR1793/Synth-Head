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
#include <Sprite.h>
#include <Composition.h>
#include <AI.h>



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


/**         TODO List
 *  Fix Radial Angular to Cartesian Approximation
 *  Add a SDF Funciton for the shader
 *  Optimise Trig Functions
 * 
 * 
 */

/*
void setup(){
    initialise_HUB75();
}

void loop(){
    drive_HUB75(0, 5, 5, 6, 5);
    BlendtoLayer(compositelayer[0],64,32,sprite,8,8,15,12,0);
    
}
*/



/*
float pi = 3.141592;//653589793238;
float SinCosApproximation[2][360] = {0};
void GenerateSinCosApproximation(){
    int TrigApproxCount = 360;
    int ArrayPos = 0;
    int CurTrigAngle = 0;
    int ExitLoop = 1;
    while(ExitLoop){
        if(TrigApproxCount <= 1){
            ExitLoop = 0;
        }
        SinCosApproximation[0][ArrayPos] = sinf((CurTrigAngle * pi)/180);
        //Serial.println(sinf((CurTrigAngle * pi)/180));
        SinCosApproximation[1][ArrayPos] = cosf((CurTrigAngle * pi)/180);
        //Serial.println(cosf((CurTrigAngle * pi)/180));
        //Serial.println("");
        ArrayPos++;
        TrigApproxCount--;
        CurTrigAngle = CurTrigAngle + 1;
    }
}

void setup() {
    initialise_HUB75();
    Serial.begin(9600);
    pinMode(33, INPUT);
    pinMode(32, INPUT);
    pinMode(23, INPUT);
    GenerateSinCosApproximation();
}
*/

// int x = 0;
// void loop(){
//     //Serial.println(cosf((38*3.1415926)/180));
// }


/*
int x = 0;
void loop(){
    Serial.println("Start");
    while(millis() < 1000){
        BlendtoLayer(compositelayer[0],64,32,sprite2,24,13,0,0,0);
        drive_HUB75(0,1,5,6,5);
        ClearDisplay(2048);
        x++;
    }

    Serial.print("Done");
    Serial.print(" ");
    Serial.println(x);
    delay(1000);

}
*/

/*
int Colour = 1;
void loop(){
    float MatrixRadius = 0;
    int AngleStep = 5;
    float OffsetY = 10.5;
    float OffsetX = 10.5;
    int CurrentAngle = 0;
    for(float i = 0; i <= 10; i = i + 0.1){
        for(int i = 0; i <= (360/AngleStep); i++){
            compositelayer[0][CartesianToLinear(round((MatrixRadius * SinCosApproximation[1][CurrentAngle])) + OffsetX, round((MatrixRadius * SinCosApproximation[0][CurrentAngle])) + OffsetY + 1)] = {Colour};
            CurrentAngle = CurrentAngle + AngleStep;
            Colour = Colour + 0b0010000001001000;
        }
        drive_HUB75(0, 5, 5, 6, 5);
        ClearDisplay(2048);

        MatrixRadius = i;
        //delay(0);
    }
    for(float i = 10; i >= 0; i = i - 0.1){
        for(int i = 0; i <= (360/AngleStep); i++){
            compositelayer[0][CartesianToLinear(round((MatrixRadius * cosf((CurrentAngle*pi)/180))) + OffsetX, round((MatrixRadius * sinf((CurrentAngle*pi)/180))) + OffsetY + 1)] = {Colour};
            CurrentAngle = CurrentAngle + AngleStep;
            Colour =+0b0010000001001000;
        }
        drive_HUB75(0, 5, 5, 6, 5);
    ClearDisplay(2048);
        MatrixRadius = i;
        //delay(0);
    }
    //delay(1000000);
    Serial.println(millis());
}



/*
void loop(){
    int MatrixRadius = 10;
    int CircleX = 0;
    int CircleY = 0;
    for(int i = 0; i < MatrixRadius; i++){
        int CircleY = sqrt(MatrixRadius*MatrixRadius - CircleX*CircleX);
        compositelayer[0][CartesianToLinear(CircleX, CircleY)] = {0x07};
        CircleX++;
    }
    for(int i = 0; i < MatrixRadius; i++){
        CircleX = sqrt(MatrixRadius*MatrixRadius - CircleY*CircleY);
        compositelayer[0][CartesianToLinear(CircleX, CircleY)] = {0x07};
        CircleY++;
    }
    drive_HUB75(0, 5, 5, 6, 5);
    //Serial.println(sqrt((10*10) - (4*4)));
}
*/


void setup(){
    initialise_HUB75();
}

void loop() { // 0.00117964 spf (847.7 fps); 0.00632083 spf w/ 5 buff colour (158.5 fps)

    //Serial.println(numtest());

//Big Test
for (size_t x = 0; x < 96; x++){
    BlendtoLayer(compositelayer[0],64,32,sprite,8,8,x,12,0);
    BlendtoLayer(compositelayer[0],64,32,sprite,8,8,x/10,17,0);
    BlendtoLayer(compositelayer[0],64,32,sprite,8,8,x/12,22,0);
    BlendtoLayer(compositelayer[0],64,32,sprite,8,8,2+x/15,12,0);
    BlendtoLayer(compositelayer[0],64,32,sprite,8,8,2+x/16,17,0);
    BlendtoLayer(compositelayer[0],64,32,sprite,8,8,2+x/5,22,0);
    BlendtoLayer(compositelayer[0],64,32,sprite,8,8,5+x/15,12,0);
    BlendtoLayer(compositelayer[0],64,32,sprite,8,8,5+x/8,17,0);
    BlendtoLayer(compositelayer[0],64,32,sprite,8,8,5+x/3.6,22,0);
    BlendtoLayer(compositelayer[0],64,32,sprite2,8,8,50-x/15,12,0);
    BlendtoLayer(compositelayer[0],64,32,sprite2,8,8,50-x/23,17,0);
    BlendtoLayer(compositelayer[0],64,32,sprite2,8,8,50-x/4,22,0);
    BlendtoLayer(compositelayer[0],64,32,sprite2,8,8,50-x/9,12,0);
    BlendtoLayer(compositelayer[0],64,32,sprite2,8,8,50-x/13,17,0);
    BlendtoLayer(compositelayer[0],64,32,sprite2,8,8,50-x/25,22,0);
    BlendtoLayer(compositelayer[0],64,32,sprite2,8,8,50-x/28,12,0);
    BlendtoLayer(compositelayer[0],64,32,sprite2,8,8,50-x/29,17,0);
    BlendtoLayer(compositelayer[0],64,32,sprite2,8,8,50-x/6,22,0);
    // bool button = digitalRead(23);
    // if(button){
    //    BlendtoLayer(compositelayer[0],64,32,sprite2,24,13,(-x/300)+20,(y/300)+2,0);
    // } else {
    //    BlendtoLayer(compositelayer[0],64,32,sprite3,24,13,(-x/300)+20,(y/300)+2,0);
    // }

    BlendtoLayer(compositelayer[0],64,32,sprite2,24,13,x,x/4,0);

    drive_HUB75(0,5,5,6,5);
    ClearDisplay(2048);
    //x++;
    //Serial.println(digitalRead(23));
    //Serial.println(x);
    //Serial.println(y);
}
}



//int x = 0;
//for (size_t i = 0; i < 63; i++)
//{
//    DrawSprite(32,sprite,8,x,12,0);
//    drive_HUB75(0,5,5,6,5);
//    int y = 0;
//    for (size_t i = 0; i < 2047; i++)
//    {
//        frame[0][y] = frame3[0][y];
//        y++;
//    }
//    
//    x++;
//}


//int x = 0;
//for(int i = 32; i >= 1; i--){
//    for(int ii = 64; ii >= 1; ii--){
//        if(compositelayer[0][x]>0){Serial.print("1");}else{Serial.print("0");};
//        //Serial.print(" ");
//        x++;
//    }
//    Serial.println("");
//}



//int x = 0;
//for(int iiii = 0; iiii <= 75; iiii++){
//    drive_HUB75(x, 5, 5, 6, 5);
//}
//for(int iiii = 0; iiii <= 6; iiii++){
//    for(int iii = 0; iii <= 2; iii++){
//        drive_HUB75(x, 5, 5, 6, 5);
//    }
//    x++;
//}
//for(int iiii = 0; iiii <= 6; iiii++){
//    for(int iii = 0; iii <= 2; iii++){
//        drive_HUB75(x, 5, 5, 6, 5);
//    }
//    x--;
//}

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