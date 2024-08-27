#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MPU6050.h>
#include <RevEng_PAJ7620.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <vector>

// Neopixel Setup
#define LED_Count 6 // Main Board (3x LEDs), Snoot Board (3x LEDs)
#define LED_Pin 13
Adafruit_NeoPixel strip(LED_Count, LED_Pin, NEO_GRBW + NEO_KHZ800);

// I2C Pins
#define SDA 21
#define SCL 22

// Sensor Setup
#define LOX2_Address 0x31
#define XSHUT_PIN 5
Adafruit_VL53L0X tof, tof2;
RevEng_PAJ7620 hgs;
Adafruit_MPU6050 mpu;

// UART Pins (ESP to ESP Comms Link)
#define RX 16
#define TX 17
HardwareSerial toDisplay(2);

// Internal Statuses
int ToF_Status = 0; // -1 = Failed to start, 0 = Null (Not initiated), 1 = Initialised, 2 = Reading
int HGS_Status = 0; // -1 = Failed to start, 0 = Null (Not initiated), 1 = Initialised, 2 = Reading
int LED_Status = 0; // -1 = Failed to start, 0 = Null (Not initiated), 1 = Initialised, 2 = Reading
int MPU_Status = 0; // -1 = Failed to start, 0 = Null (Not initiated), 1 = Initialised, 2 = Reading
uint64_t CommandStatus;

// Internal Variables
uint16_t tof_distance = 0;
uint16_t tof_distance2 = 0;
unsigned long previousMillis = 0; // Store the last time LED was updated
const long interval = 500; // Interval at which to toggle the LED (milliseconds)

std::vector<int> scanI2C() {
    std::vector<int> addresses;

    Serial.println("Scanning I2C bus...");

    for (int address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");

            addresses.push_back(address);
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }

    if (addresses.empty()) {
        Serial.println("No I2C devices found\n");
    } else {
        Serial.println("Done\n");
    }

    return addresses;
}

std::vector<int> scanI2C1() {
    std::vector<int> addresses;

    Serial.println("Scanning I2C bus...");

    for (int address = 1; address < 127; address++) {
        Wire1.beginTransmission(address);
        byte error = Wire1.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");

            addresses.push_back(address);
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }

    if (addresses.empty()) {
        Serial.println("No I2C devices found\n");
    } else {
        Serial.println("Done\n");
    }

    return addresses;
}

// Function to modify specified bits in originalValue
void modifyBits(uint64_t& originalValue, uint8_t startBit, uint8_t endBit, uint64_t valueToPutIn) {
    // Check if startBit and endBit are within the valid range for a uint64_t
    if (startBit >= 64 || endBit >= 64 || startBit > endBit) {
        // Handle invalid bit range, if needed
        return;
    }

    // Calculate the bit length of the value to put in
    uint8_t bitLength = endBit - startBit + 1;

    // Create a mask for the bit length
    uint64_t mask = ((1ULL << bitLength) - 1) << startBit;

    // Clear the bits in the original value at the specified range
    originalValue &= ~mask;

    // Align the value to put in with the start bit
    uint64_t alignedValue = valueToPutIn << startBit;

    // Apply the new value to the original value
    originalValue |= (alignedValue & mask);
}

void setup() {
    // Begin UART Serial
    toDisplay.begin(1500000, SERIAL_8N1, RX, TX);
    double temp = 0;
    for(int i = 0; i < 8; i ++){toDisplay.print(temp);}
    modifyBits(CommandStatus,0,0,0); // Booting Up;
    toDisplay.print(temp);

    pinMode(12, OUTPUT); // FAN
    pinMode(XSHUT_PIN, OUTPUT);

    pinMode(25, INPUT); // Photoresistor 1
    pinMode(26, INPUT); // Photoresistor 2
    pinMode(27, INPUT); // Photoresistor 3
    pinMode(14, INPUT); // Photoresistor 4

    // Begin Serial
    Serial.begin(115200);
    Serial.println("Begun Serial");

    // Begin I2C
    Wire.begin(SDA, SCL);
    Wire1.begin(18,19);
    // Wire.setClock(50000);
    scanI2C();
    scanI2C1();
    Serial.println("after");
    // delay(1000);

    // digitalWrite(XSHUT_PIN, LOW);

    // Attempt to initialize the sensor
    if(!tof2.begin(0x29, false, &Wire1)) {
        Serial.println("TOF 2 Failed to Start!");
    }else{
        Serial.println("TOF 2 Sensor initialized successfully.");
        tof2.startRangeContinuous();
    }
    // delay(10);

    digitalWrite(XSHUT_PIN, HIGH);

    delay(10);

    if(!tof.begin(0x29)){
        Serial.println("TOF Failed to Start!");
    }else{
        Serial.println("TOF Sensor initialized successfully.");
        tof.startRangeContinuous();
    }
    delay(10);

    // digitalWrite(X)

    // Begin HGS
    if(!hgs.begin(&Wire1)){ HGS_Status = -1; }
    else { HGS_Status = 1; }
    hgs.setGestureEntryTime(50);
    hgs.setGestureExitTime(200);

    // Begin MPU6050
    if (!mpu.begin()) {
        MPU_Status = -1;
    } else {
        MPU_Status = 1;
        // mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
        mpu.setMotionDetectionThreshold(1);
        mpu.setMotionDetectionDuration(20);
        mpu.setInterruptPinLatch(true);    // Keep it latched. Will turn off when reinitialized.
        mpu.setInterruptPinPolarity(true);
        mpu.setMotionInterrupt(true);
    }

    // Begin LED Strip
    strip.begin();
    strip.show();
    strip.setBrightness(200);
    scanI2C();
}

// Helper functions for color wheel generation
uint8_t WheelRed(uint16_t wheelPos) {
    if (wheelPos < 85) {
        return wheelPos * 3;
    } else if (wheelPos < 170) {
        return 255 - (wheelPos - 85) * 3;
    } else {
        return 0;
    }
}

uint8_t WheelGreen(uint16_t wheelPos) {
    if (wheelPos < 85) {
        return 255 - wheelPos * 3;
    } else if (wheelPos < 170) {
        return 0;
    } else {
        return (wheelPos - 170) * 3;
    }
}

uint8_t WheelBlue(uint16_t wheelPos) {
    if (wheelPos < 85) {
        return 0;
    } else if (wheelPos < 170) {
        return (wheelPos - 85) * 3;
    } else {
        return 255 - (wheelPos - 170) * 3;
    }
}

// Function to generate a color wheel index for hue-changing LEDs
uint32_t Wheel(unsigned long millis) {
    static uint16_t wheelIndex = 0;
    wheelIndex = (millis / 50) % 255;
    return strip.Color(WheelRed(wheelIndex), WheelGreen(wheelIndex), WheelBlue(wheelIndex));
}

// Gravity Compensation
double SLX, SHX, SLY, SHY, SLZ, SHZ;
void processSensorData(double sensorValue, double EMA_a_low, double EMA_a_high, double& EMA_S_low, double& EMA_S_high, double& bandpass) {
  // Calculate Exponential Moving Averages (EMAs)
  EMA_S_low = (EMA_a_low * sensorValue) + ((1 - EMA_a_low) * EMA_S_low);
  EMA_S_high = (EMA_a_high * sensorValue) + ((1 - EMA_a_high) * EMA_S_high);

  // Calculate highpass and bandpass outputs
//   highpass = sensorValue - EMA_S_low;
  bandpass = EMA_S_high - EMA_S_low;
}

void printBits(double value) {
  // Use a union to reinterpret the double's memory as bytes
  union {
    double d;
    byte b[sizeof(double)];
  } u;

  u.d = value;

  // Print each byte in the union as binary
  for (int i = sizeof(double) - 1; i >= 0; i--) {
    for (int j = 7; j >= 0; j--) {
      Serial.print((u.b[i] >> j) & 1);
    }
    Serial.print(" ");
  }
  Serial.println();
}

uint8_t animation = 0;

int gesturetimer = 0;
int lastgesture = 0;
uint8_t gesture = 0;

void loop() {
    // toDisplay.println("Ping");
    unsigned long currentMillis = millis();

    // if (currentMillis - previousScanTime >= scanInterval) {
    //     previousScanTime = currentMillis;

    //     // Perform the I2C scan
    //     scanI2C();
    // }

    // Update LED status and read sensor data every 20 milliseconds
    if (currentMillis - previousMillis >= 10) {
        previousMillis = currentMillis;

        // Read ToF distance
        VL53L0X_RangingMeasurementData_t measurement, measurement2;
        tof.rangingTest(&measurement, false); // false: async ranging
        tof_distance = measurement.RangeMilliMeter;
        
        // delay(100);
        tof2.rangingTest(&measurement2, false);
        tof_distance2 = measurement2.RangeMilliMeter;
        // Serial.print("TOF 1: ");
        // Serial.print(tof_distance);
        // Serial.print(" TOF 2: ");
        // Serial.print(measurement2.RangeMilliMeter);
        // Serial.println("");
        // scanI2C();

        // Read MPU6050 accelerometer if motion detected
        if (mpu.getMotionInterruptStatus()) {
            sensors_event_t accel, gyro, temp;
            mpu.getEvent(&accel, &gyro, &temp);
        } else {
        }

        // Read HGS gesture
        // 1 Up, 2 Down, 3 Left, 4 Right, 5 Forward, 6 Backwards, 7 Clockwise, 8 Anticlockwise
        gesture = hgs.readGesture();
        switch (gesture) {
            case GES_UP:
                Serial.println("Gesture: UP");
                break;
            case GES_DOWN:
                Serial.println("Gesture: DOWN");
                break;
            case GES_LEFT:
                Serial.println("Gesture: LEFT");
                break;
            case GES_RIGHT:
                Serial.println("Gesture: RIGHT");
                break;
            case GES_FORWARD:
                Serial.println("Gesture: FORWARD");
                break;
            case GES_BACKWARD:
                Serial.println("Gesture: BACKWARD");
                break;
            case GES_CLOCKWISE:
                Serial.println("Gesture: CLOCKWISE");
                break;
            case GES_ANTICLOCKWISE:
                Serial.println("Gesture: ANTICLOCKWISE");
                break;
            default:
                // Serial.println("Gesture: UNKNOWN");
                break;
        }


        // Print gesture and ToF distance
        // Serial.print("Detected Gesture: ");
        // Serial.print(gestureName);
        // Serial.print(", ToF Distance: ");
        // Serial.print(tof_distance);
        // Serial.println(" mm");

        // Show updated LED status
        strip.show();
    }

    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    // if(temp.temperature > 15){
    //     digitalWrite(12, HIGH);
    // }else{
    //     digitalWrite(12, LOW);
    // }

    // digitalWrite(12);

    double xtest,ytest,ztest;
    processSensorData(accel.acceleration.x, 0.13, 0.14, SLX, SHX, xtest);
    processSensorData(accel.acceleration.y, 0.13, 0.14, SLY, SHY, ytest);
    processSensorData(accel.acceleration.z, 0.13, 0.14, SLZ, SHZ, ztest);

    double brightness1 = (map(analogRead(25), 1, 4095, 0, 100)+map(analogRead(26), 1, 4095, 0, 100))/2;
    double brightness2 = (map(analogRead(27), 1, 4095, 0, 100)+map(analogRead(14), 1, 4095, 0, 100))/2;
    // Serial.print(brightness1);
    // Serial.print(" ");
    // Serial.print(brightness2);
    // Serial.println("");

    /**
     * @brief Uart Display Data Structure
     * 
     * Gravity Compensated & Smoothed XYZ Acceleration Data
     *      AccelX, AccelY, AccelZ
     * Smoothed XYZ Gyroscope Data
     *      GyroX, GyroY, GyroZ
     * Brightnesses
     *      Panel1, Panel2
     * System Status (Whats Alive) : Commands [Single 64bit Value]
     *      Status Bits: ESP Booting Up, HGS, ToF1, ToF2, PResist1, PResist2, PResist3, PResist4
     *      Command Bits: Restart Display, Turn On/Off, Restart Display (Parity), Turn On/Off (Parity)
     *      Command 4Bits: Menu Select (Vertical), Menu Select (Horizontal), Expansion (N/A)
     *      Command 8Bits: Animation/Display ID
     * 
     * 
     * @brief Animation ID List:
     *      
     */

    CommandStatus = 0;

    modifyBits(CommandStatus, 0, 0, 1); // ESP Running;
    modifyBits(CommandStatus, 1, 1, 1); // HGS Alive : Not Implemented Yet
    modifyBits(CommandStatus, 2, 2, 1); // ToF1 Alive : Not Implemented Yet
    modifyBits(CommandStatus, 3, 3, 1); // ToF2 Alive : Not Implemented Yet
    modifyBits(CommandStatus, 4, 4, 1); // Photoresistor Alive : Not Implemented Yet
    modifyBits(CommandStatus, 5, 5, 1); // Photoresistor Alive : Not Implemented Yet
    modifyBits(CommandStatus, 6, 6, 1); // Photoresistor Alive : Not Implemented Yet
    modifyBits(CommandStatus, 7, 7, 1); // Photoresistor Alive : Not Implemented Yet

    modifyBits(CommandStatus, 8, 8, 0); // Dont Restart ESP32 : Not Implemented Yet
    modifyBits(CommandStatus, 9, 9, 1); // Display On/Off : Not Implemented Yet
    modifyBits(CommandStatus, 10, 10, 0); // Dont Restart ESP32 : Not Implemented Yet
    modifyBits(CommandStatus, 11, 11, 1); // Display On/Off : Not Implemented Yet

    modifyBits(CommandStatus, 12, 15, 0); // Menu Select (Vertical) : Not Implemented Yet
    modifyBits(CommandStatus, 16, 19, 0); // Menu Select (Horizontal) : Not Implemented Yet
    modifyBits(CommandStatus, 20, 23, 0); // Expansion (N/A)

    double gx = gyro.gyro.x, gy = gyro.gyro.y, gz = gyro.gyro.z;

    if(tof_distance>30){
        modifyBits(CommandStatus, 24, 31, 0);
        strip.setPixelColor(3, strip.Color(0, 0, 0)); // 4th LED
        strip.setPixelColor(5, strip.Color(0, 0, 0)); // 6th LED
    }else{
        modifyBits(CommandStatus, 24, 31, 1);
        strip.setPixelColor(3, strip.Color(255, 0, 0)); // 4th LED
        strip.setPixelColor(5, strip.Color(255, 0, 0)); // 6th LED
    }

    if(tof_distance2>130){
        modifyBits(CommandStatus, 24, 31, 2);
        digitalWrite(12, LOW);
    }else{
        digitalWrite(12, HIGH);
    }
if (gesture == 3) {
    gesturetimer = 500;
    Serial.println("Menu Started");
}
if ((gesture > 0) && (gesturetimer > 0)) {
    Serial.println("Menued");
    gesturetimer = 500;
    if (gesture == GES_UP) lastgesture = 1;
    if (gesture == GES_DOWN) lastgesture = 2;
    if (gesture == GES_LEFT) lastgesture = 3;
    if (gesture == GES_RIGHT) lastgesture = 4;
    if (gesture == GES_FORWARD) lastgesture = 5;
    if (gesture == GES_BACKWARD) lastgesture = 6;
    if (gesture == GES_CLOCKWISE) lastgesture = 7;
    if (gesture == GES_ANTICLOCKWISE) lastgesture = 8;
}

    switch (lastgesture)
    {
    case 2:
        gx = 0; gy = 0; gz = 0; xtest = 0; ytest = 0; ztest = 0;
        Serial.println("Case 2");
        break;
    case 4:
        modifyBits(CommandStatus, 24, 31, 3);
        Serial.println("Case 4");
    default:
        break;
    }
    gesturetimer--;
    if(gesturetimer<0){gesturetimer = 0;}
    if(gesturetimer>500){gesturetimer = 500;}


    // Serial.println(tof_distance);

    toDisplay.print(xtest*100); toDisplay.print(" "); // Acceleration x
    toDisplay.print(ytest*100); toDisplay.print(" "); // Acceleration y
    toDisplay.print(ztest*100); toDisplay.print(" "); // Acceleration z
    toDisplay.print(gyro.gyro.x); toDisplay.print(" "); // Gyroscope x
    toDisplay.print(gyro.gyro.y); toDisplay.print(" "); // Gyroscope y
    toDisplay.print(gyro.gyro.z); toDisplay.print(" "); // Gyroscope z
    toDisplay.print(brightness1); toDisplay.print(" "); // Panel 1 Brightness
    toDisplay.print(brightness2); toDisplay.print(" "); // Panel 2 Brightness
    toDisplay.print(CommandStatus); toDisplay.print(" "); // 64 Bit Status & Command
    toDisplay.println("");
}
