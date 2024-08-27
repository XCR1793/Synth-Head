#include <Arduino.h>
#include <ESP32_Servo.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
Servo TailServos[4];
// HardwareSerial fromTransmitter(2);
int ServoPins[] = {19, 18, 5, 17};

const int numReadings = 10;  // Number of readings to include in the moving average
float gyroXReadings[numReadings];
float gyroYReadings[numReadings];
int gyroXIndex = 0;
int gyroYIndex = 0;
const float gyroThreshold = 2.0;  // Threshold to reject small noise

void addToMovingAverage(float *readings, int &index, float newValue) {
    readings[index] = newValue;
    index = (index + 1) % numReadings;
}

float calculateMovingAverage(float *readings) {
    float sum = 0;
    for (int i = 0; i < numReadings; i++) {
        sum += readings[i];
    }
    return sum / numReadings;
}

const int centerPosition1 = 120;
const int centerPosition2 = 120;
const int centerPosition0 = 150;
const int centerPosition3 = 150;

void moveServo(Servo &servo, int centerPosition, int offset) {
    int targetPosition = centerPosition + offset;
    servo.write(targetPosition);
    delay(10);  // Delay for smoother motion
}


void setup() {
    Serial.begin(115200);
    Wire.begin(16, 4);
    for (size_t i = 0; i < 4; i++) {
        TailServos[i].attach(ServoPins[i]);
    }
    // fromTransmitter.begin(115200, SERIAL_8N1, 22, 21);

    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1);
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);

}

// unsigned long lastCheckTime = 0;
// const unsigned long checkInterval = 10000;
// unsigned long currentTime;
// void PingEverySecond(){
//     currentTime = millis();
//     if(currentTime - lastCheckTime >= checkInterval){
//         fromTransmitter.begin(115200, SERIAL_8N1, 21, 22);
//         lastCheckTime = currentTime;
//     }
// }

int Idle = 0;
int Mode = 1;
int Restart = 0;
void loop() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // PingEverySecond();

    // if(fromTransmitter.available()){
    //     float idle, mode, restart;
    //     String buffer = fromTransmitter.readStringUntil('\n');
    //     sscanf(buffer.c_str(), "%f %f %f", &idle, &mode, &restart);
    //     Idle = static_cast<int>(idle);
    //     Mode = static_cast<int>(mode);
    //     Restart = static_cast<int>(restart);
    // }
    // Serial.print(Idle);
    // Serial.print(" ");
    // Serial.print(Mode);
    // Serial.print(" ");
    // // Serial.println(Restart);
    // Serial.print(digitalRead(22));
    // Serial.print(" ");
    // Serial.print(digitalRead(21));
    // Serial.println("");

    if(1){
        switch(1){
            case 0: // Wag
                TailServos[2].write(150);
                TailServos[1].write(150);
                for(signed int i = -45; i <= 45; i++){
                    TailServos[0].write(120 + i);
                    TailServos[3].write(120 - i*2);
                    delay(5);
                }
                for(signed int i = 45; i >= -45; i--){
                    TailServos[0].write(120 + i);
                    TailServos[3].write(120 - i*2);
                    delay(5);
                }
                break;
            case 1: // Gyro
                // Serial.println(g.gyro.z);
                TailServos[0].write(120 + g.gyro.z * 15);
                TailServos[3].write(120 - g.gyro.z * 25);
                break;
            default:
                break;
        }
    }
    // if(Restart){
    //     Serial.print("ESP Restarting...");
    //     TailServos[2].write(150);
    //     TailServos[1].write(150);
    //     TailServos[0].write(120);
    //     TailServos[3].write(120);
    //     ESP.restart();
    // }
    // Serial.println(g.gyro.z);


    // Move servo 1 and 2 (center position: 120 degrees) in opposite directions
    // for(signed int i = -45; i <= 45; i++){
    //     TailServos[2].write(120 + i);
    //     TailServos[1].write(120 - i*2);
    //     delay(20);
    // }
    // for(signed int i = -45; i <= 45; i++){
    //     TailServos[3].write(150 + i);
    //     TailServos[0].write(150 - i*2);
    //     delay(20);
    // }
    // for(signed int i = 45; i >= -45; i--){
    //     TailServos[2].write(120 + i);
    //     TailServos[1].write(120 - i*2);
    //     delay(20);
    // }
    // for(signed int i = 45; i >= -45; i--){
    //     TailServos[3].write(150 + i);
    //     TailServos[0].write(150 - i*2);
    //     delay(20);
    // }

}