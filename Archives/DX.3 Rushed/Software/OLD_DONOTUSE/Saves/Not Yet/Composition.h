#include <Arduino.h>

#pragma once

/**
 * To do
 *  - Edge Detection
 *  - LUT for Circle Values (only 1 quarter)
 *  - Inside/Outside Detection
 *  - Pixel 3x3 Sampling
 *  - Signed Distance Field
 *  - Compositor
 */




// Circle LUT
#ifndef MatrixSize
    const int MatrixWidth = 64;
    const int MatrixHeight = 32;
#endif

#define MatrixArea = MatrixHeight * MatrixWidth;

int CircleLUT[MatrixWidth * MatrixHeight] = {0};
int LinearToCartesianReturn[2] = {0};

int CartesianToLinear(int XPos, int YPos){
    int HeightArea = (YPos - 1) * MatrixWidth;
    return HeightArea + XPos;
}

void LinearToCartesian(int String){
    int HeightCount = 0;
    while(String > MatrixWidth){
        String =- MatrixWidth;
        HeightCount++;
    }
    int WidthCount = String;
    LinearToCartesianReturn[1] = {MatrixHeight};
    LinearToCartesianReturn[2] = {MatrixWidth};
}

void CreateCircleLUT(int MatrixCircleRadius){
    int MatrixRadius = sqrt(MatrixHeight * MatrixWidth);
    int CircleX = 0;
    for(int i = 0; i <= MatrixCircleRadius; i++){
        int CircleY = sqrt(MatrixRadius - CircleX^2);
        compositelayer[0][CartesianToLinear(CircleX, CircleY)] = {0x07};
        Serial.println(CartesianToLinear(CircleX, CircleY));
        CircleX++;
    }

}

void Hue(int Hue, int RedMax, int GreenMax, int BlueMax){
    
}