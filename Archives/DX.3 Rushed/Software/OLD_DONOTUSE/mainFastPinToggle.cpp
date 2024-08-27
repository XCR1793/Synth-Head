#include <Arduino.h>
#include <driver/adc.h>

void setup(){
    // Serial.begin(115200);
    pinMode(2, OUTPUT);
}

void loop(){
    for(size_t i = 0; i <= 1000; i++){
        REG_WRITE(GPIO_OUT_W1TS_REG, (1 << 2));  // Set the pin
        REG_WRITE(GPIO_OUT_W1TC_REG, (1 << 2));  // Clear the pin
    }
}