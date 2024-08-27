#include <Arduino.h>
#include <Wire.h>
#include <Display.h>
#include <ImageTest.h>

HUB75 Panels;
Compose compose;
Shader Modify;

float colourcycle = 0;
float colourvelocity = 0;
int velocitycycle = 0;
float colourcycle2 = 0;
float colourvelocity2 = 0;
int velocitycycle2 = 0;

void printDateTime() {
  Wire.beginTransmission(0x68); // DS1307 RTC I2C address
  Wire.write((byte)0x00); // Set the register pointer to 0 (seconds)
  Wire.endTransmission();
  delay(1000); // Wait for the DS1307 to respond

  Wire.requestFrom(0x68, 7); // Request data from the DS1307 (seconds, minutes, hours, day, date, month, year)
  if (Wire.available()) {
    int second = Wire.read();
    int minute = Wire.read();
    int hour = Wire.read();
    int day = Wire.read();
    int date = Wire.read();
    int month = Wire.read();
    int year = Wire.read() + 2000; // The DS1307 stores the year with two digits, so add 2000

    Serial.print("Date: ");
    Serial.print(year);
    Serial.print("-");
    if (month < 10) Serial.print('0');
    Serial.print(month);
    Serial.print("-");
    if (date < 10) Serial.print('0');
    Serial.print(date);
    Serial.println();

    Serial.print("Time: ");
    if (hour < 10) Serial.print('0');
    Serial.print(hour);
    Serial.print(":");
    if (minute < 10) Serial.print('0');
    Serial.print(minute);
    Serial.print(":");
    if (second < 10) Serial.print('0');
    Serial.println(second);
  } else {
    Serial.println("DS1307 not responding");
  }
}

void Idle() {
  Panels.Print("CREATED BY", 0x0801, 5, 4, 87, 18);
  Panels.Print("XCR-1793", 0x0801, 5, 4, 95, 26);

  printDateTime();

  // Rest of your Idle function code
}

void setup() {
  Serial.begin(9600);
  Panels.PanelCount(2);
  Panels.Buffer(5);
  Panels.Initialise();
  for (size_t i = 0; i <= 500; i++) {
    // Rest of your setup code
  }
}

void loop() {
  Idle();
  Panels.drive_HUB75();
  Panels.Clear();
}
