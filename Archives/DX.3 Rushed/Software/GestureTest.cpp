/**
 * @file GestureTest.cpp
 * @brief Simple test to check if the gesture sensor works along with showing how to use it in software
 * @version 0.1
 * @date 2024-06-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <Wire.h>
#include "RevEng_PAJ7620.h"

#define SDA_PIN 21
#define SCL_PIN 22

RevEng_PAJ7620 sensor = RevEng_PAJ7620();

void setup() {
    Wire.begin(SDA_PIN, SCL_PIN);  // Initialize I2C on specified pins
    Serial.begin(115200);

    sensor.begin();  // Initialize the PAJ7620U2 sensor

    // Optional: Set gesture timing parameters
    sensor.setGestureEntryTime(50);  // Example entry time adjustment (in ms)
    sensor.setGestureExitTime(200);  // Example exit time adjustment (in ms)
}

void loop() {
    Gesture gesture = sensor.readGesture();

    if (gesture != GES_NONE) {
        Serial.print("Detected Gesture: ");
        switch (gesture) {
            case GES_UP:
                Serial.println("UP");
                break;
            case GES_DOWN:
                Serial.println("DOWN");
                break;
            case GES_LEFT:
                Serial.println("LEFT");
                break;
            case GES_RIGHT:
                Serial.println("RIGHT");
                break;
            case GES_FORWARD:
                Serial.println("FORWARD");
                break;
            case GES_BACKWARD:
                Serial.println("BACKWARD");
                break;
            case GES_CLOCKWISE:
                Serial.println("CLOCKWISE");
                break;
            case GES_ANTICLOCKWISE:
                Serial.println("ANTICLOCKWISE");
                break;
            case GES_WAVE:
                Serial.println("WAVE");
                break;
            default:
                Serial.println("UNKNOWN");
                break;
        }
    }

    delay(100);  // Adjust delay as needed
}
