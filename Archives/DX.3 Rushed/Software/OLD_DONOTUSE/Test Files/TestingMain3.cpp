#include <Arduino.h>
#include <driver/adc.h>

const int numSamples = 10; // Number of samples to average


void setup() {
  Serial.begin(9600);
}

void loop() {
  int total = 0;
  for (int i = 0; i < numSamples; i++) {
    total += hallRead();
    delay(10); // Adjust the delay as needed between readings
  }

  int average = total / numSamples;
  Serial.printf("Average sensor value: %d\n", average);

  delay(250); // Adjust the delay as needed between average readings
}
