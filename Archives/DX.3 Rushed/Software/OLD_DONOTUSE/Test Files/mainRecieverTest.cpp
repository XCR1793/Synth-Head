#include <Arduino.h>
#include <Wire.h>

HardwareSerial fromTransmitter(2);

unsigned long lastCheckTime = 0;
const unsigned long checkInterval = 10000;
unsigned long currentTime;

void PingEverySecond() {
    currentTime = millis();
    if (currentTime - lastCheckTime >= checkInterval) {
        fromTransmitter.begin(115200, SERIAL_8N1, 22, 21);
        lastCheckTime = currentTime;
    }
}

void setup() {
    Serial.begin(115200);
    fromTransmitter.begin(115200, SERIAL_8N1, 22, 21);
}

void loop() {
    if (fromTransmitter.available()) {
        String buffer = fromTransmitter.readStringUntil('\n');
        float idle, mode;
        sscanf(buffer.c_str(), "%f %f", &idle, &mode);

        Serial.print("Idle: ");
        Serial.print(idle);
        Serial.print(" Mode: ");
        Serial.print(mode);
        Serial.println("");
    }
    PingEverySecond();
}
