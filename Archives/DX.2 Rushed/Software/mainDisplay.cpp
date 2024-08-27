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
#include <ImageTest.h>
#include <Display.h>

#include <Wire.h>
#include <driver/adc.h>

const int numSamples = 10;

HardwareSerial fromTransmitter(2);

Shader Modify;
HUB75 Panels;
Compose compose;

const int amplitude = 3;   // Peak amplitude (0-255)
const float frequency = 0.25;    // Frequency in Hz
const float frequency2 = 0.3;
const int offset = 10;      // Offset (shifts the wave up or down)

#define GYRO_MOVING_AVERAGE_SIZE 10  // Adjust this to your desired window size

float gyroXValues[GYRO_MOVING_AVERAGE_SIZE];
float gyroYValues[GYRO_MOVING_AVERAGE_SIZE];
int gyroValueIndex = 0;

// Function to calculate the moving average
float calculateMovingAverage(float data[], int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

float colourcycle = 0;
float colourvelocity = 0;
int velocitycycle = 0;
float colourcycle2 = 0;
float colourvelocity2 = 0;
int velocitycycle2 = 0;

void setup(){
    Serial.begin(115200);
    fromTransmitter.begin(115200, SERIAL_8N1, 21, 22);
    Panels.PanelCount(2);
    Panels.Buffer(5);
    Panels.Initialise();
    for(size_t i = 0; i <= 500; i++){
        compose.BlendtoLayer(Panels.Array, 128, 32, CrowLogo, 38, 28, 5, 3, 1);
        Panels.Print("CREATED BY XCR-1793", 0xf800, 5, 4, 42, 10);
            for(size_t k = 0; k <= 127; k++){
            colourcycle2 = colourvelocity2 + i;
            for(size_t j = 0; j <= 31; j++){
                if(Panels.Array[k+(j*128)]==0xf800){Panels.DrawPixel(k, j+1, Modify.HueReturnColour(colourcycle2));}
                colourcycle2++;
            }
        }
        Panels.drive_HUB75();
        Panels.Clear();
    }
}

unsigned long lastCheckTime = 0;
const unsigned long checkInterval = 10000;
unsigned long currentTime;
void PingEverySecond(){
    currentTime = millis();
    if(currentTime - lastCheckTime >= checkInterval){
        fromTransmitter.begin(115200, SERIAL_8N1, 21, 22);
        lastCheckTime = currentTime;
    }
}

float gyroX, gyroY, gyroZ;
int idle, TLSToggle, restart, Anim;
int AnimCounter = 0;
int total = 0;
int average = 0;
void loop(){
    if (fromTransmitter.available() >= 12) {
        String buffer = fromTransmitter.readStringUntil('\n');
        sscanf(buffer.c_str(), "%f %f %f %d %d %d %d", &gyroX, &gyroY, &gyroZ, &idle, &TLSToggle, &restart, &Anim);
    }
    if(restart > 0){ESP.restart();}
    for(size_t i = 0; i < numSamples; i++){
        total += hallRead();
    }
    average = total / numSamples;
    total = 0;
    // Serial.println(hallRead());
    PingEverySecond();
    if(average > 65){
        if(!idle){

            // Apply moving average to gyro values
            gyroXValues[gyroValueIndex] = gyroX;
            gyroYValues[gyroValueIndex] = gyroY;

            // Increment the index, and loop back to the beginning if necessary
            gyroValueIndex = (gyroValueIndex + 1) % GYRO_MOVING_AVERAGE_SIZE;

            // Calculate the moving average for gyro values
            float gyroXAvg = calculateMovingAverage(gyroXValues, GYRO_MOVING_AVERAGE_SIZE);
            float gyroYAvg = calculateMovingAverage(gyroYValues, GYRO_MOVING_AVERAGE_SIZE);

            unsigned long currentTime = millis();

            switch(AnimCounter){
                case 0:
                    compose.BlendtoLayer(Panels.Array, 128, 32, sprite2, 24, 13, 16 + gyroXAvg + amplitude * sin(2 * PI * frequency2 * currentTime / 1000.0), 10 + gyroYAvg - amplitude * sin(2 * PI * frequency * currentTime / 1000.0), 0);
                    compose.BlendtoLayer(Panels.Array, 128, 32, sprite3, 24, 13, 64 + 24 - gyroXAvg - amplitude * sin(2 * PI * frequency2 * currentTime / 1000.0), 10 - gyroYAvg -amplitude * sin(2 * PI * frequency * currentTime / 1000.0), 0);
                    break;
                case 1:
                    compose.BlendtoLayer(Panels.Array, 128, 32, sprite2, 24, 13, 16 + gyroXAvg * 2, 10 + gyroYAvg * 2, 0);
                    compose.BlendtoLayer(Panels.Array, 128, 32, sprite3, 24, 13, 64 + 24 - gyroXAvg * 2, 10 - gyroYAvg * 2, 0);
                    break;
                default:
                    break;
            }

            if(Anim > 0){AnimCounter ++;};
            if(AnimCounter > 1){AnimCounter = 0;}
            
            if(TLSToggle){
                for(size_t i = 0; i <= 63; i++){
                    colourcycle = colourvelocity + i;
                    for(size_t j = 0; j <= 31; j++){
                        if(Panels.Array[i+(j*128)]){Panels.DrawPixel(i, j+1, Modify.HueReturnColour(colourcycle));}
                        colourcycle++;
                    }
                }
                for(size_t i = 0; i <= 63; i++){
                    colourcycle2 = colourvelocity2 - i;
                    for(size_t j = 0; j <= 31; j++){
                        if(Panels.Array[64+i+(j*128)]){Panels.DrawPixel(64+i, j+1, Modify.HueReturnColour(colourcycle2));}
                        colourcycle2++;
                    }
                }
                velocitycycle++;
                if(!(velocitycycle%5)){
                    colourvelocity++;
                }
                velocitycycle2++;
                if(!(velocitycycle2%5)){
                    colourvelocity2++;
                }
            }

            Panels.drive_HUB75();
            Panels.Clear();
            //colourcycle = colourvelocity;
        }else{
            Panels.Print("IDLE", 0x0801, 8, 12, 12, 20);
            Panels.Print("IDLE", 0x0801, 8, 12, 12 + 64, 20);
            Panels.drive_HUB75();
            Panels.Clear();
        }
    }else{
        Panels.Print("VISOR", 0xFFFF, 8, 12, 5, 4);
        Panels.Print("OPEN",  0xFFFF, 8, 12, 10, 22);
        Panels.Print("VISOR", 0xFFFF, 8, 12, 5+64, 4);
        Panels.Print("OPEN",  0xFFFF, 8, 12, 10+64, 22);
        Panels.drive_HUB75();
        Panels.Clear();
    }
}
