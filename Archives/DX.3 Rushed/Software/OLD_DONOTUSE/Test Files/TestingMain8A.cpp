#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(2); // Use mySerial for ESP32 #2

String buffer = "";

void setup() {
  Serial.begin(115200); // Initialize the hardware serial for debugging
  mySerial.begin(9600, SERIAL_8N1, 21, 22); // Initialize mySerial for ESP32 #2 on pins 4 (RX) and 5 (TX)
}

void loop() {
  while (mySerial.available()) {
    char receivedChar = mySerial.read();
    if (receivedChar == '\n') {
      // Print the received data when a newline is received
      Serial.println("Received data: " + buffer);
      buffer = ""; // Clear the buffer
    } else {
      // Add the character to the buffer
      buffer += receivedChar;
    }
  }
}
