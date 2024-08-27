#include <Arduino.h>
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(22, 21);
}

void loop() {
  while (true) {
    Wire.beginTransmission(0x68); // DS1307 RTC I2C address
    Wire.write((byte)0x00); // Set the register pointer to 0 (seconds)
    Wire.endTransmission();

    Wire.requestFrom(0x68, 1); // Request data from the DS1307 (only seconds)
    if (Wire.available()) {
      int second = Wire.read();
      Serial.print("Seconds: ");
      if (second < 10) Serial.print('0');
      Serial.println(second, DEC);
    } else {
      Serial.println("DS1307 not responding");
    }

    delay(1000); // Delay for one second before reading again
  }
}
