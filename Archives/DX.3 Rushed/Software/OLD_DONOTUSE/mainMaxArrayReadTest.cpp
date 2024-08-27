#include <Arduino.h>
#include <driver/adc.h>

int imagearray[4096] = {};

void setup() {
    pinMode(0, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(32, OUTPUT);
    pinMode(23, OUTPUT);
}

void updateGPIO(int pin, bool value) {
    if (value) {
        REG_WRITE(GPIO_OUT_W1TS_REG, (1 << pin));
    } else {
        REG_WRITE(GPIO_OUT_W1TC_REG, (1 << pin));
    }
}

bool switchcase = 0;
void loop() {
    updateGPIO(23, 1);
    for(size_t i = 0; i <= 4095; i++){
        if(((imagearray[i] = 0)) >= 1){updateGPIO(0 ,switchcase);}else{updateGPIO(0 ,switchcase);}
        if(((imagearray[i] = 0)) >= 1){updateGPIO(4 ,switchcase);}else{updateGPIO(4 ,switchcase);}
        if(((imagearray[i] = 0)) >= 1){updateGPIO(5 ,switchcase);}else{updateGPIO(5 ,switchcase);}
        if(((imagearray[i] = 0)) >= 1){updateGPIO(14,switchcase);}else{updateGPIO(14,switchcase);}
        if(((imagearray[i] = 0)) >= 1){updateGPIO(12,switchcase);}else{updateGPIO(12,switchcase);}
        if(((imagearray[i] = 0)) >= 1){updateGPIO(32,switchcase);}else{updateGPIO(32,switchcase);}
    }
    updateGPIO(23, 0);
    for(size_t i = 0; i <= 4095; i++){
        if(((imagearray[i] = 0)) >= 1){updateGPIO(0 ,switchcase);}else{updateGPIO(0 ,switchcase);}
        if(((imagearray[i] = 0)) >= 1){updateGPIO(4 ,switchcase);}else{updateGPIO(4 ,switchcase);}
        if(((imagearray[i] = 0)) >= 1){updateGPIO(5 ,switchcase);}else{updateGPIO(5 ,switchcase);}
        if(((imagearray[i] = 0)) >= 1){updateGPIO(14,switchcase);}else{updateGPIO(14,switchcase);}
        if(((imagearray[i] = 0)) >= 1){updateGPIO(12,switchcase);}else{updateGPIO(12,switchcase);}
        if(((imagearray[i] = 0)) >= 1){updateGPIO(32,switchcase);}else{updateGPIO(32,switchcase);}
    }
}
