#include <Arduino.h>

void setup(){
    Serial.begin(115200);
    pinMode(22, INPUT);
    pinMode(21, INPUT);
}

void loop(){
    Serial.print("Pin 22: ");
    Serial.print(digitalRead(22));
    Serial.print("  Pin 21: ");
    Serial.print(digitalRead(21));
    Serial.println("");
}