/**
 * @file GestureToFTest.cpp
 * @brief Simple test to test how both sensors are used at the same time
 * @version 0.1
 * @date 2024-06-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <Wire.h>
#include "Adafruit_VL53L0X.h"
#include "RevEng_PAJ7620.h"

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_VL53L0X lox;
RevEng_PAJ7620 sensor;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(1); // wait for Serial port to connect
    }

    Wire.begin(SDA_PIN, SCL_PIN);  // Initialize I2C on specified pins

    // Initialize VL53L0X sensor
    if (!lox.begin()) {
        Serial.println(F("Failed to boot VL53L0X"));
        while (1);
    }
    Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));
    lox.startRangeContinuous();  // Start continuous ranging mode

    // Initialize PAJ7620 sensor
    sensor.begin();
    sensor.setGestureEntryTime(50);  // Example entry time adjustment (in ms)
    sensor.setGestureExitTime(200);  // Example exit time adjustment (in ms)
}

void loop() {
    static uint16_t previousDistance = 0;
    uint16_t currentDistance = 0;

    // Read distance from VL53L0X sensor if a new measurement is available
    if (lox.isRangeComplete()) {
        currentDistance = lox.readRange();
        
        // Print distance only if it changes by more than 1mm
        if (abs(currentDistance - previousDistance) > 1) {
            Serial.print("Distance in mm: ");
            Serial.println(currentDistance);
            previousDistance = currentDistance;
        }
    }

    // Read gesture from PAJ7620 sensor
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
