#include <Arduino.h>

#include <HardwareSerial.h>

HardwareSerial mySerial(2); // Use mySerial for ESP32 #1

void setup() {
  Serial.begin(115200); // Initialize the hardware serial for debugging
  mySerial.begin(9600, SERIAL_8N1, 4, 5); // Initialize mySerial for ESP32 #1 on pins 4 (RX) and 5 (TX)
}

void loop() {
  mySerial.println("Hello from ESP32 #1");
  delay(1000);
}
