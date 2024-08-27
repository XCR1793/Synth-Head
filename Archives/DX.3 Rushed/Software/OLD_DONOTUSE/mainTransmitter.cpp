#include <Wire.h>
#include <HardwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>
#include <driver/adc.h>
#include <WiFi.h>

#define LED_Pin 12
#define LED_Count 11
#define LED_Brightness 255

// 26 25 23

const int averageSamples = 10;
int movingtotal = 0;
int movingaverage = 0;

Adafruit_MPU6050 mpu;
HardwareSerial toDisplay(2);
// HardwareSerial toTail(1);

// NeoPixel settings
Adafruit_NeoPixel strip(LED_Count, LED_Pin, NEO_GRBW + NEO_KHZ800);

const float lowerThreshold = 26.0;
const float upperThreshold = 27.0;
bool isAboveUpperThreshold = false;
bool isBelowLowerThreshold = true;

void setup() {
    Serial.begin(115200);
    pinMode(14, INPUT);
    pinMode(27, INPUT);
    pinMode(26, INPUT);
    pinMode(25, INPUT);
    pinMode(17, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(32, OUTPUT);
    toDisplay.begin(115200, SERIAL_8N1, 4, 5);
    // toTail.begin(115200, SERIAL_8N1, 33, 32);
    Wire.begin(22, 21);

    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1);
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);

    strip.begin();
    strip.show();  // Initialize all pixels to 'off'
    strip.setBrightness(LED_Brightness);
}

void addToMovingAverage(float *readings, int &index, float newValue){
    readings[index] = newValue;
    index = (index + 1) % averageSamples;
}

float calculateMovingAverage(float *readings){
    float sum = 0;
    for(int i = 0; i < averageSamples; i++){
        sum += readings[i];
    }
    return sum / averageSamples;
}

int lastButtonState = HIGH;  // Previous state of the button
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // Adjust the debounce delay as needed

bool checkForRisingEdge(int pin){
  int buttonState = !digitalRead(pin);
  unsigned long currentTime = millis();

  if(buttonState == HIGH && lastButtonState == LOW && (currentTime - lastDebounceTime) > debounceDelay) {
    lastButtonState = buttonState;
    lastDebounceTime = currentTime;
    return true;
  }

  lastButtonState = buttonState;
  return false;
}

bool buttonStateB = LOW;
bool prevButtonStateB = LOW;
bool isFallingEdgeB(int pin){
    bool newButtonStateB = digitalRead(pin);
    bool fallingEdgeB = !newButtonStateB && prevButtonStateB;
    prevButtonStateB = newButtonStateB;
    return fallingEdgeB;
}

bool ButtonState = 0;
bool ButtonToggler(int pin){
  if(isFallingEdgeB(pin)){
    ButtonState = !ButtonState;
  }
  return ButtonState;
}

// unsigned long lastCheckTime = 0;
// const unsigned long checkInterval = 1000;
// unsigned long currentTime;
// void PingEverySecond(){
//     currentTime = millis();
//     if(currentTime - lastCheckTime >= checkInterval){
//         toTail.begin(115200, SERIAL_8N1, 33, 32);
//         lastCheckTime = currentTime;
//     }
// }



bool ButtonToggled = 0;
bool Reset = 0;
bool LEDDir = 0;
int brightnessamount = 0;
int IdleThresh = 300;
void loop() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    toDisplay.print(g.gyro.x);
    toDisplay.print(" ");
    toDisplay.print(g.gyro.y);
    toDisplay.print(" ");
    toDisplay.print(g.gyro.z);
    toDisplay.print(" ");
    if((analogRead(14) > IdleThresh)){toDisplay.print("0"); digitalWrite(32, HIGH);}else{toDisplay.print("1"); digitalWrite(32, LOW);}
    toDisplay.print(" ");
    // toTail.print(" ");
    if(ButtonToggler(27)){toDisplay.print("1"); digitalWrite(33, HIGH);}else{toDisplay.print("0"); digitalWrite(33, LOW);} // TLS on/off
    toDisplay.print(" ");
    if(!digitalRead(26)){toDisplay.print("1"); Reset = 1;}else{toDisplay.print("0"); Reset = 0;} // Restart
    toDisplay.print(" ");
    toDisplay.print(checkForRisingEdge(25)); // Next Animation
    toDisplay.println("");
    
    // if((analogRead(14) > 600)){toTail.print("0");}else{toTail.print("1");}
    // toTail.print(" ");
    // toTail.print("1");
    // toTail.print(" ");
    // toTail.print(Reset);
    // toTail.println("");
    if(Reset){digitalWrite(33, HIGH); digitalWrite(32, HIGH);}
    if(Reset){ESP.restart();}
    // PingEverySecond();


    if (temp.temperature >= upperThreshold && !isAboveUpperThreshold){
        isAboveUpperThreshold = true;
        isBelowLowerThreshold = false;
    } else if (temp.temperature <= lowerThreshold && !isBelowLowerThreshold){
        isAboveUpperThreshold = false;
        isBelowLowerThreshold = true;
    }

    if (isAboveUpperThreshold){
        digitalWrite(17, HIGH);
        strip.fill(strip.Color(255, 0, 0, 180));
    } else if (isBelowLowerThreshold){
        digitalWrite(17, LOW);
        strip.fill(strip.Color(0, 0, 255, 180));
    }
    Serial.print("Temperature: ");
    Serial.print(temp.temperature);

    if((analogRead(14) < IdleThresh)){
        if(!LEDDir){
            brightnessamount++;
            if (brightnessamount >= 255){
                LEDDir = true;
            }
        }else {
            brightnessamount--;
            if (brightnessamount == 0){
                LEDDir = false;
            }
        }

    strip.fill(strip.Color(0, 0, 0, brightnessamount));
    strip.show();
  }

    movingtotal = 0;
    for (int i = 0; i < averageSamples; i++) {
        movingtotal += hallRead();
    }
    movingaverage = movingtotal / averageSamples;

    strip.show();
    Serial.print("  Lidar: ");
    Serial.println(analogRead(14));


}
