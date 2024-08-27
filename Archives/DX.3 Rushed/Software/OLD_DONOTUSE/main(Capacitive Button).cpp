#include <Arduino.h>

void setup(){
    Serial.begin(115200);
    pinMode(A0, INPUT);
}

void loop(){
    Serial.println(analogRead(A0));
    delay(200);
}