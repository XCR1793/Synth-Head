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

class HUB75{
    protected:
    int Display_Pins[14] = {19, 25, 18, 5, 26, 17, 16, 14, 4, 12, 27, 13, 2, 0}; // R0, B0, G0, R1, G1, B1, A, B, C, D, E, Latch, Output Enable, Clock
    int LED_Matrix_Panel_Count = 2;
    int LED_Matrix_Height = 32;
    int LED_Matrix_Width = 64;
    int LED_Matrix_Scan_Rate = 16;
    int LED_Matrix_Brightness = 255;
    int LED_Matrix_Buffer = 5;
    int Colour_Space[4] = {5, 6, 5, 0}; // R, G, B, A  || Default Value is the 565 RGB Colour space with no alpha
    int Panel_Direction[2] = {0, 0}; // Upside Down, Left to Right

    int PanelSize = LED_Matrix_Height * LED_Matrix_Width;

    int* CreateArray1D(int size){
        int* NewArray = new int[size];
        for(int i = 0; i < size; i++){
            NewArray[i] = 0;
        }
        return NewArray;
    }

    int** CreateArray2D(int size1, int size2){      // Creates New 2D Array
        int** NewArray = new int*[size1];
        for (int i = 0; i < size1; i++){
            NewArray[i] = new int[size2];
            for (int j = 0; j < size2; j++){
                NewArray[i][j] = 0;
            }
        }
        return NewArray;
    }

    bool Bit_Return(int Number, int Bit){         // Number = numbers, Bit = 0010 (2nd bit return) || Function returns the bit in position
        if(Number & (1 << Bit)){return 1;}
        return 0;
    }

    void Column_Select(int ColumnNumber){
        if(Bit_Return(ColumnNumber,0)){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[ 6]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[ 6]));}
        if(Bit_Return(ColumnNumber,1)){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[ 7]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[ 7]));}
        if(Bit_Return(ColumnNumber,2)){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[ 8]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[ 8]));}
        if(Bit_Return(ColumnNumber,3)){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[ 9]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[ 9]));}
        if(Bit_Return(ColumnNumber,4)){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[10]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[10]));}
    }

    void Circle_Plot(int Center_x, int Center_y, int Position_x, int Position_y, int Colour) {
        DrawPixel(Center_x + Position_x, Center_y + Position_y, Colour);
        DrawPixel(Center_x - Position_x, Center_y + Position_y, Colour);
        DrawPixel(Center_x + Position_x, Center_y - Position_y, Colour);
        DrawPixel(Center_x - Position_x, Center_y - Position_y, Colour);
        DrawPixel(Center_x + Position_y, Center_y + Position_x, Colour);
        DrawPixel(Center_x - Position_y, Center_y + Position_x, Colour);
        DrawPixel(Center_x + Position_y, Center_y - Position_x, Colour);
        DrawPixel(Center_x - Position_y, Center_y - Position_x, Colour);
    }


    void CharacterA(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-0)), start_x + (maxsize*2), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(8-8)), start_x + (maxsize*4), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-4)), start_x + (maxsize*3), start_y + (maxsize*(8-4)), colour);
    }
    void CharacterB(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        int TrueStart_x = 8 - start_x;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-0)), start_x + (maxsize*0), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-8)), start_x + (maxsize*2), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(8-8)), start_x + (maxsize*4), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-7)), start_x + (maxsize*4), start_y + (maxsize*(8-5)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-5)), start_x + (maxsize*2), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(8-4)), start_x + (maxsize*4), start_y + (maxsize*(8-3)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-3)), start_x + (maxsize*4), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-1)), start_x + (maxsize*2), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(8-0)), start_x + (maxsize*0), start_y + (maxsize*(8-0)), colour);
    }
    void CharacterC(int start_x, int start_y, int size, int colour){
        float maxsize = size/4.00;
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(4-4)), start_x + (maxsize*1), start_y + (maxsize*(4-4)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-4)), start_x + (maxsize*0), start_y + (maxsize*(4-3)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-3)), start_x + (maxsize*0), start_y + (maxsize*(4-1)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-1)), start_x + (maxsize*1), start_y + (maxsize*(4-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-0)), start_x + (maxsize*2), start_y + (maxsize*(4-0)), colour);
    }
    void CharacterD(int start_x, int start_y, int size, int colour){
        float maxsize = size/4.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-0)), start_x + (maxsize*0), start_y + (maxsize*(4-4)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-4)), start_x + (maxsize*1), start_y + (maxsize*(4-4)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-4)), start_x + (maxsize*2), start_y + (maxsize*(4-3)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(4-3)), start_x + (maxsize*2), start_y + (maxsize*(4-1)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(4-1)), start_x + (maxsize*1), start_y + (maxsize*(4-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-0)), start_x + (maxsize*0), start_y + (maxsize*(4-0)), colour);
    }
    void CharacterE(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*0), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-1)), start_x + (maxsize*1), start_y + (maxsize*(2-1)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-0)), colour);
    }
    void CharacterF(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*0), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-1)), start_x + (maxsize*1), start_y + (maxsize*(2-1)), colour);
    }
    void CharacterG(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(8-4)), start_x + (maxsize*4), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-4)), start_x + (maxsize*4), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-1)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*1), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-0)), start_x + (maxsize*0), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-1)), start_x + (maxsize*0), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-7)), start_x + (maxsize*1), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-8)), start_x + (maxsize*3), start_y + (maxsize*(8-8)), colour);
    }
    void CharacterH(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*0), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-1)), start_x + (maxsize*1), start_y + (maxsize*(2-1)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
    }
    void CharacterI(int start_x, int start_y, int size, int colour){
        float maxsize = size/4.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-0)), start_x + (maxsize*2), start_y + (maxsize*(4-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-0)), start_x + (maxsize*1), start_y + (maxsize*(4-4)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-4)), start_x + (maxsize*2), start_y + (maxsize*(4-4)), colour);
    }
    void CharacterJ(int start_x, int start_y, int size, int colour){
        float maxsize = size/4.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-0)), start_x + (maxsize*1), start_y + (maxsize*(4-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-0)), start_x + (maxsize*2), start_y + (maxsize*(4-1)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(4-1)), start_x + (maxsize*2), start_y + (maxsize*(4-2)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(4-2)), start_x + (maxsize*2), start_y + (maxsize*(4-4)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(4-4)), start_x + (maxsize*0), start_y + (maxsize*(4-4)), colour);
    }
    void CharacterK(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*0), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-1)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-1)), start_x + (maxsize*1), start_y + (maxsize*(2-0)), colour);
    }
    void CharacterL(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*0), start_y + (maxsize*(2-0)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-0)), colour);
    }
    void CharacterM(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-0)), start_x + (maxsize*1), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-8)), start_x + (maxsize*2), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(8-0)), start_x + (maxsize*3), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-8)), start_x + (maxsize*4), start_y + (maxsize*(8-0)), colour);
    }
    void CharacterN(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*0), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*1), start_y + (maxsize*(2-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
    }
    void CharacterO(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-1)), start_x + (maxsize*0), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-7)), start_x + (maxsize*1), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-8)), start_x + (maxsize*3), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-8)), start_x + (maxsize*4), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-7)), start_x + (maxsize*4), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-1)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*1), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-0)), start_x + (maxsize*0), start_y + (maxsize*(8-1)), colour);
    }
    void CharacterP(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*0), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(2-2)), start_x + (maxsize*1), start_y + (maxsize*(2-1)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-1)), start_x + (maxsize*1), start_y + (maxsize*(2-1)), colour);
    }
    void CharacterQ(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-1)), start_x + (maxsize*0), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-7)), start_x + (maxsize*1), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-8)), start_x + (maxsize*3), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-8)), start_x + (maxsize*4), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-7)), start_x + (maxsize*4), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-1)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*1), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-0)), start_x + (maxsize*0), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(8-2)), start_x + (maxsize*4), start_y + (maxsize*(8-0)), colour);
    }
    void CharacterR(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*0), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(2-2)), start_x + (maxsize*1), start_y + (maxsize*(2-1)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-1)), start_x + (maxsize*1), start_y + (maxsize*(2-1)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-1)), start_x + (maxsize*1), start_y + (maxsize*(2-0)), colour);
    }
    void CharacterS(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*0), start_y + (maxsize*(2-1)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(2-1)), start_x + (maxsize*0), start_y + (maxsize*(2-1)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(2-1)), start_x + (maxsize*1), start_y + (maxsize*(2-0)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-0)), colour);
    }
    void CharacterT(int start_x, int start_y, int size, int colour){
        float maxsize = size/4.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-4)), start_x + (maxsize*2), start_y + (maxsize*(4-4)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-4)), start_x + (maxsize*1), start_y + (maxsize*(4-0)), colour);
    }
    void CharacterU(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*0), start_y + (maxsize*(2-0)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
    }
    void CharacterV(int start_x, int start_y, int size, int colour){
        float maxsize = size/4.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-4)), start_x + (maxsize*1), start_y + (maxsize*(4-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-0)), start_x + (maxsize*2), start_y + (maxsize*(4-4)), colour);
    }
    void CharacterW(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-8)), start_x + (maxsize*1), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-0)), start_x + (maxsize*2), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*2), start_y + (maxsize*(8-8)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*4), start_y + (maxsize*(8-8)), colour);
    }
    void CharacterX(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*1), start_y + (maxsize*(2-0)), colour);
    }
    void CharacterY(int start_x, int start_y, int size, int colour){
        float maxsize = size/4.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-4)), start_x + (maxsize*1), start_y + (maxsize*(4-2)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-2)), start_x + (maxsize*2), start_y + (maxsize*(4-4)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-2)), start_x + (maxsize*1), start_y + (maxsize*(4-0)), colour);
    }
    void CharacterZ(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-2)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(2-2)), start_x + (maxsize*0), start_y + (maxsize*(2-0)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-0)), colour);
    }
    void Character0(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-1)), start_x + (maxsize*0), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-7)), start_x + (maxsize*1), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-8)), start_x + (maxsize*3), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-8)), start_x + (maxsize*4), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-7)), start_x + (maxsize*4), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-1)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*1), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-0)), start_x + (maxsize*0), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-1)), start_x + (maxsize*4), start_y + (maxsize*(8-7)), colour);
    }
    void Character1(int start_x, int start_y, int size, int colour){
        float maxsize = size/4.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-3)), start_x + (maxsize*1), start_y + (maxsize*(4-4)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(4-4)), start_x + (maxsize*1), start_y + (maxsize*(4-0)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(4-0)), start_x + (maxsize*2), start_y + (maxsize*(4-0)), colour);
    }
    void Character2(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-6)), start_x + (maxsize*0), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-7)), start_x + (maxsize*1), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-8)), start_x + (maxsize*3), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-8)), start_x + (maxsize*4), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-7)), start_x + (maxsize*4), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-4)), start_x + (maxsize*0), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-0)), start_x + (maxsize*4), start_y + (maxsize*(8-0)), colour);
    }
    void Character3(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-8)), start_x + (maxsize*3), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-8)), start_x + (maxsize*4), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-7)), start_x + (maxsize*4), start_y + (maxsize*(8-5)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-5)), start_x + (maxsize*3), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-4)), start_x + (maxsize*1), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-4)), start_x + (maxsize*4), start_y + (maxsize*(8-3)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-3)), start_x + (maxsize*4), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-1)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*0), start_y + (maxsize*(8-0)), colour);
    }
    void Character4(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-8)), start_x + (maxsize*0), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-4)), start_x + (maxsize*4), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-8)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
    }
    void Character5(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-8)), start_x + (maxsize*0), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-8)), start_x + (maxsize*0), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-4)), start_x + (maxsize*3), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-4)), start_x + (maxsize*4), start_y + (maxsize*(8-3)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-3)), start_x + (maxsize*4), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-1)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*0), start_y + (maxsize*(8-0)), colour);
    }
    void Character6(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-1)), start_x + (maxsize*0), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-7)), start_x + (maxsize*1), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-8)), start_x + (maxsize*3), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-4)), start_x + (maxsize*0), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-4)), start_x + (maxsize*4), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-1)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*1), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-0)), start_x + (maxsize*0), start_y + (maxsize*(8-1)), colour);
    }
    void Character7(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(2-2)), start_x + (maxsize*0), start_y + (maxsize*(2-2)), colour);
    }
    void Character8(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-1)), start_x + (maxsize*0), start_y + (maxsize*(8-3)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-3)), start_x + (maxsize*1), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-4)), start_x + (maxsize*0), start_y + (maxsize*(8-5)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-5)), start_x + (maxsize*0), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-8)), start_x + (maxsize*3), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-8)), start_x + (maxsize*4), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-7)), start_x + (maxsize*4), start_y + (maxsize*(8-5)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-5)), start_x + (maxsize*3), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-4)), start_x + (maxsize*1), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-4)), start_x + (maxsize*4), start_y + (maxsize*(8-3)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-3)), start_x + (maxsize*4), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-1)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*1), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-0)), start_x + (maxsize*0), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-7)), start_x + (maxsize*1), start_y + (maxsize*(8-8)), colour);
    }
    void Character9(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-1)), start_x + (maxsize*1), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-0)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*4), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-1)), start_x + (maxsize*4), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*4), start_y + (maxsize*(8-7)), start_x + (maxsize*3), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-8)), start_x + (maxsize*1), start_y + (maxsize*(8-8)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-8)), start_x + (maxsize*0), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-7)), start_x + (maxsize*0), start_y + (maxsize*(8-5)), colour);
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(8-5)), start_x + (maxsize*1), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-4)), start_x + (maxsize*3), start_y + (maxsize*(8-4)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-4)), start_x + (maxsize*4), start_y + (maxsize*(8-5)), colour);
    }
    void CharacterDot(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-0)), start_x + (maxsize*1), start_y + (maxsize*(8-2)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-2)), start_x + (maxsize*3), start_y + (maxsize*(8-2)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-2)), start_x + (maxsize*3), start_y + (maxsize*(8-0)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-0)), start_x + (maxsize*1), start_y + (maxsize*(8-0)), colour);
    }
    void CharacterHyphen(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-4)), start_x + (maxsize*3), start_y + (maxsize*(8-4)), colour);
    }
    void CharacterSlash(int start_x, int start_y, int size, int colour){
        float maxsize = size/2.00;
        DrawLine(start_x + (maxsize*0), start_y + (maxsize*(2-0)), start_x + (maxsize*1), start_y + (maxsize*(2-2)), colour);
    }
    void CharacterColon(int start_x, int start_y, int size, int colour){
        float maxsize = size/8.00;
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-1)), start_x + (maxsize*1), start_y + (maxsize*(8-3)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-3)), start_x + (maxsize*3), start_y + (maxsize*(8-3)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-3)), start_x + (maxsize*3), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-1)), start_x + (maxsize*1), start_y + (maxsize*(8-1)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-5)), start_x + (maxsize*1), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*1), start_y + (maxsize*(8-7)), start_x + (maxsize*3), start_y + (maxsize*(8-7)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-7)), start_x + (maxsize*3), start_y + (maxsize*(8-5)), colour);
        DrawLine(start_x + (maxsize*3), start_y + (maxsize*(8-5)), start_x + (maxsize*1), start_y + (maxsize*(8-5)), colour);
    }

    public:
    int* Array; // Write to this array if you want the Driver to see it

    void SetPins(int HUB75_R0, int HUB75_B0, int HUB75_G0, int HUB75_R1, int HUB75_G1, int HUB75_B1, int HUB75_A, int HUB75_B, int HUB75_C, int HUB75_D, int HUB75_E, int HUB75_Latch, int HUB75_Output_Enable, int HUB75_Clock){
        Display_Pins[0] = HUB75_R0;
        Display_Pins[1] = HUB75_B0;
        Display_Pins[2] = HUB75_G0;
        Display_Pins[3] = HUB75_R1;
        Display_Pins[4] = HUB75_G1;
        Display_Pins[5] = HUB75_B1;
        Display_Pins[6] = HUB75_A;
        Display_Pins[7] = HUB75_B;
        Display_Pins[8] = HUB75_C;
        Display_Pins[9] = HUB75_D;
        Display_Pins[10] = HUB75_E;
        Display_Pins[11] = HUB75_Latch;
        Display_Pins[12] = HUB75_Output_Enable;
        Display_Pins[13] = HUB75_Clock;
    }
    
    void PinOverride(int Pin, int _0to13){Display_Pins[_0to13] = Pin;}

    void SetSize(int Height, int Width){LED_Matrix_Height = Height; LED_Matrix_Width = Width;}

    void SetScan(int scanrate){LED_Matrix_Scan_Rate = scanrate;}

    void PanelCount(int Panels){LED_Matrix_Panel_Count = Panels;}

    void Brightness(int _0to255){LED_Matrix_Brightness = _0to255;} // Not Implemented Yet
    
    void Buffer(int BufferCount){LED_Matrix_Buffer = BufferCount;}
    
    void ColourSpace(int Red, int Green, int Blue, int Alpha){Colour_Space[0] = Red; Colour_Space[0] = Green; Colour_Space[0] = Blue; Colour_Space[0] = Alpha;}

    void Clear(){for(size_t i = 0; i <= (LED_Matrix_Height * LED_Matrix_Width * LED_Matrix_Panel_Count); i++){Array[i] = 0;}}

    void UpsideDown(){Panel_Direction[1] = 1;}

    void DrawPixel(int x, int y, int colour){if(x < LED_Matrix_Width*LED_Matrix_Panel_Count){Array[x+((y-1)*LED_Matrix_Width*LED_Matrix_Panel_Count)] = colour;}}

    void DrawLine(int Position_1_x, int Position_1_y, int Position_2_x, int Position_2_y, int Colour){
        int dx = abs(Position_2_x - Position_1_x);
        int dy = abs(Position_2_y - Position_1_y);
        int sx, sy;
        if(Position_1_x < Position_2_x){ sx = 1;}else{ sx = -1;}
        if(Position_1_y < Position_2_y){ sy = 1;}else{ sy = -1;}
        int err = dx - dy;
        while(true){
            DrawPixel(Position_1_x, Position_1_y, Colour);
            if(Position_1_x == Position_2_x && Position_1_y == Position_2_y){ break;}
            int e2 = 2 * err;
            if(e2 > -dy){err -= dy; Position_1_x += sx;}
            if(e2 < dx){err += dx; Position_1_y += sy;}
        }
    }

    void DrawCircle(int Center_x, int Center_y, int Radius, int Colour) {
        int x = 0;
        int y = Radius;
        int NextPixel = 1 - Radius;
        Circle_Plot(Center_x, Center_y, x, y, Colour);
        while (x < y) {
            x++;
            if(NextPixel < 0){NextPixel += 2 * x + 1;}else{y--; NextPixel += 2 * (x - y) + 1;}
            Circle_Plot(Center_x, Center_y, x, y, Colour);
        }
    }

    void Print(const char* sentence, int colour, int size, int spacing, int xpos, int ypos){
        int positionx = xpos;
        int positiony = ypos;
        for(int i = 0; sentence[i] != '\0'; i++){
            switch(sentence[i]){
                case 'A': CharacterA(positionx, positiony, size, colour); break;
                case 'B': CharacterB(positionx, positiony, size, colour); break;
                case 'C': CharacterC(positionx, positiony, size, colour); break;
                case 'D': CharacterD(positionx, positiony, size, colour); break;
                case 'E': CharacterE(positionx, positiony, size, colour); break;
                case 'F': CharacterF(positionx, positiony, size, colour); break;
                case 'G': CharacterG(positionx, positiony, size, colour); break;
                case 'H': CharacterH(positionx, positiony, size, colour); break;
                case 'I': CharacterI(positionx, positiony, size, colour); break;
                case 'J': CharacterJ(positionx, positiony, size, colour); break;
                case 'K': CharacterK(positionx, positiony, size, colour); break;
                case 'L': CharacterL(positionx, positiony, size, colour); break;
                case 'M': CharacterM(positionx, positiony, size, colour); break;
                case 'N': CharacterN(positionx, positiony, size, colour); break;
                case 'O': CharacterO(positionx, positiony, size, colour); break;
                case 'P': CharacterP(positionx, positiony, size, colour); break;
                case 'Q': CharacterQ(positionx, positiony, size, colour); break;
                case 'R': CharacterR(positionx, positiony, size, colour); break;
                case 'S': CharacterS(positionx, positiony, size, colour); break;
                case 'T': CharacterT(positionx, positiony, size, colour); break;
                case 'U': CharacterU(positionx, positiony, size, colour); break;
                case 'V': CharacterV(positionx, positiony, size, colour); break;
                case 'W': CharacterW(positionx, positiony, size, colour); break;
                case 'X': CharacterX(positionx, positiony, size, colour); break;
                case 'Y': CharacterY(positionx, positiony, size, colour); break;
                case 'Z': CharacterZ(positionx, positiony, size, colour); break;
                case '0': Character0(positionx, positiony, size, colour); break;
                case '1': Character1(positionx, positiony, size, colour); break;
                case '2': Character2(positionx, positiony, size, colour); break;
                case '3': Character3(positionx, positiony, size, colour); break;
                case '4': Character4(positionx, positiony, size, colour); break;
                case '5': Character5(positionx, positiony, size, colour); break;
                case '6': Character6(positionx, positiony, size, colour); break;
                case '7': Character7(positionx, positiony, size, colour); break;
                case '8': Character8(positionx, positiony, size, colour); break;
                case '9': Character9(positionx, positiony, size, colour); break;
                case '.': CharacterDot(positionx, positiony, size, colour); break;
                case '-': CharacterHyphen(positionx, positiony, size, colour); break;
                case '/': CharacterSlash(positionx, positiony, size, colour); break;
                case ':': CharacterColon(positionx, positiony, size, colour); break;

                default:
                // positionx += spacing/2;
                break;
            }
            positionx = positionx + spacing;
        }
    }
    // void PrintSimple(){}
    // void Print(){}


    // HelloWorld(){}
    // OWO(){}
    // SimpleSynth(){}
    // SimpleProtogen(){}

    void Initialise(){
        Array = CreateArray1D(LED_Matrix_Height * LED_Matrix_Width * LED_Matrix_Panel_Count);

        for(size_t i = 0; i <= (sizeof(Display_Pins) / sizeof(Display_Pins[0])); i++){
            pinMode(Display_Pins[i], OUTPUT);
        }
    }

    void drive_HUB75() {    // Not fully optimised

        int RColourSub = 1;
        int GColourSub = 1;
        int BColourSub = 1;

        int CurrentBuff = 0;

        for(int i = 0; i < Colour_Space[0]; i++){RColourSub = RColourSub * 2;}; RColourSub = RColourSub/(LED_Matrix_Buffer);
        for(int i = 0; i < Colour_Space[1]; i++){GColourSub = GColourSub * 2;}; GColourSub = GColourSub/(LED_Matrix_Buffer);
        for(int i = 0; i < Colour_Space[2]; i++){BColourSub = BColourSub * 2;}; BColourSub = BColourSub/(LED_Matrix_Buffer);

        int RBitMask = 0;
        int GBitMask = 0;
        int BBitMask = 0;
        for(int i = 0; i < Colour_Space[0]; i++){RBitMask = RBitMask << 1; RBitMask = RBitMask | 0b1;} RBitMask = RBitMask << (Colour_Space[1] + Colour_Space[2]);
        for(int i = 0; i < Colour_Space[1]; i++){GBitMask = GBitMask << 1; GBitMask = GBitMask | 0b1;} GBitMask = GBitMask << (Colour_Space[2]);
        for(int i = 0; i < Colour_Space[2]; i++){BBitMask = BBitMask << 1; BBitMask = BBitMask | 0b1;}

        for(int i = 1; i <= LED_Matrix_Buffer; i++){
        
            int latcount = 0;
            int scan = 16;
            int cycle = 0;
            int cycle2 = LED_Matrix_Height * LED_Matrix_Width;

            for(int ii = 1; ii <= scan; ii++){
                REG_WRITE(GPIO_OUT_W1TC_REG, (1 << Display_Pins[11]));
                REG_WRITE(GPIO_OUT_W1TS_REG, (1 << Display_Pins[11]));
                REG_WRITE(GPIO_OUT_W1TC_REG, (1 << Display_Pins[12]));

                for(int ii = 1; ii <= LED_Matrix_Width*LED_Matrix_Panel_Count; ii++) {
                
                    // ESP32 Compatible
                    if(1 <= ((Array[cycle ] & RBitMask)>>(Colour_Space[1] + Colour_Space[2]))-RColourSub*CurrentBuff){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[0]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[0]));}
                    if(1 <= ((Array[cycle ] & GBitMask)>>(Colour_Space[2])                  )-GColourSub*CurrentBuff){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[1]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[1]));}
                    if(1 <= ((Array[cycle ] & BBitMask)                                     )-BColourSub*CurrentBuff){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[2]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[2]));}
                    if(1 <= ((Array[cycle2] & RBitMask)>>(Colour_Space[1] + Colour_Space[2]))-RColourSub*CurrentBuff){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[3]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[3]));}
                    if(1 <= ((Array[cycle2] & GBitMask)>>(Colour_Space[2])                  )-GColourSub*CurrentBuff){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[4]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[4]));}
                    if(1 <= ((Array[cycle2] & BBitMask)                                     )-BColourSub*CurrentBuff){REG_WRITE(GPIO_OUT_W1TS_REG,(1<<Display_Pins[5]));}else{REG_WRITE(GPIO_OUT_W1TC_REG,(1<<Display_Pins[5]));}

                    // Arduino Compatible
                    // if(1 <= ((TestArray[cycle ] & RBitMask)>>(Gvalues + Bvalues))-RColourSub*CurrentBuff){digitalWrite(HUBR0,HIGH);} else {digitalWrite(HUBR0,LOW);}
                    // if(1 <= ((TestArray[cycle ] & GBitMask)>>(Bvalues)          )-GColourSub*CurrentBuff){digitalWrite(HUBG0,HIGH);} else {digitalWrite(HUBG0,LOW);}
                    // if(1 <= ((TestArray[cycle ] & BBitMask)                     )-BColourSub*CurrentBuff){digitalWrite(HUBB0,HIGH);} else {digitalWrite(HUBB0,LOW);}
                    // if(1 <= ((TestArray[cycle2] & RBitMask)>>(Gvalues + Bvalues))-RColourSub*CurrentBuff){digitalWrite(HUBR1,HIGH);} else {digitalWrite(HUBR1,LOW);}
                    // if(1 <= ((TestArray[cycle2] & GBitMask)>>(Bvalues)          )-GColourSub*CurrentBuff){digitalWrite(HUBG1,HIGH);} else {digitalWrite(HUBG1,LOW);}
                    // if(1 <= ((TestArray[cycle2] & BBitMask)                     )-BColourSub*CurrentBuff){digitalWrite(HUBB1,HIGH);} else {digitalWrite(HUBB1,LOW);}

                    REG_WRITE(GPIO_OUT_W1TC_REG, (1 << Display_Pins[13]));
                    REG_WRITE(GPIO_OUT_W1TS_REG, (1 << Display_Pins[13]));

                    cycle = cycle + 1;
                    cycle2 = cycle2 + 1;

                }
                REG_WRITE(GPIO_OUT_W1TS_REG, (1 << Display_Pins[12]));

                Column_Select(latcount);

                latcount = latcount + 1;
            }
            CurrentBuff ++;
        }
    }
};

class Compose{      //  Blending & Shite
    private:
    int blendMode(int mainpixel, int compositepixel, int blendMode){//Add more blend modes later
        switch(blendMode){
            case 0:
                return compositepixel;
                break;
            case 1:
                if(!mainpixel){return compositepixel;}else{return mainpixel;}
                break;
            default:
            break;
        }
        return 0;
    }

    public:
    void BlendtoLayer(int previousLayer[], int prelayerwidth, int prelayerheight, int spriteArray[], int spritewidth, int spriteheight, int xoffset, int yoffset, int blendmode){
        int totaloffset = (yoffset*prelayerwidth)+xoffset;
        int loopover = prelayerwidth-(spritewidth);

        int spritecount = 0;
        for(size_t i = 1; i <= spriteheight; i++){
            for(size_t i = 1; i <= spritewidth; i++){
                previousLayer[totaloffset] = blendMode(previousLayer[totaloffset], spriteArray[spritecount], blendmode);
                spritecount++;
                totaloffset++;
            }
            totaloffset = totaloffset + loopover;
        }
    }
    
    /*
    BlendModes // Different blendmodes
    Interpolate // Interpolate sprite so it moves smoothly
    */
};

class Shader : HUB75, Compose{       //  Top Level Shader or TLS Shader: Cool effects that can be applied to the image along with masking n stuff
    private:
    public:
    uint16_t HueReturnColour(int hue){
        hue = hue % 360;
        int h = (hue * 6) % 360;
        int sector = h / 60;
        int f = (h % 60) * 255 / 60;
        uint8_t r, g, b;
        switch (sector) {
            case 0: r = 255;    g = f;      b = 0;      break;
            case 1: r = 255 - f;g = 255;    b = 0;      break;
            case 2: r = 0;      g = 255;    b = f;      break;
            case 3: r = 0;      g = 255 - f;b = 255;    break;
            case 4: r = f;      g = 0;      b = 255;    break;
            case 5: r = 255;    g = 0;      b = 255 - f;break;
        }
        return ((r >> (8 - HUB75::Colour_Space[0])) << (HUB75::Colour_Space[1] + HUB75::Colour_Space[2])) | ((g >> (8 - HUB75::Colour_Space[1])) << HUB75::Colour_Space[2]) | (b >> (8 - HUB75::Colour_Space[2]));;
    }
    /*
    ScreenGradient // Gradient over any colour part of the screen
    Glitch // Horizontal random scattering with chromatic aberration
    */
};

class Sprites{      //  Sprite & Shapes that can be pasted onto the display
    private:
    public:
    /*
    Some Shapes n stuff
    */
};
