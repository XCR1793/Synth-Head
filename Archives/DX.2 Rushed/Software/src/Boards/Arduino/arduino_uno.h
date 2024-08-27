#pragma once
#define arduino_uno

// Standard Arduino Uno Setup

#ifdef arduino_uno

#define I2C_Count_Available 2

#ifdef I2C_0
#define I2C_SDA0 28
#define I2C_SCL0 
#endif

#ifdef I2C_1
#define I2C_SDA1
#define I2C_SCL1
#endif


#endif