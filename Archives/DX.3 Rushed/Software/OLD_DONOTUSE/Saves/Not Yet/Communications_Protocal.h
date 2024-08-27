/**
 *  All communication protocals including I2C, SPI and CAN
*/
#pragma once

#include <Arduino.h>

// I2C Data Communication Protocal
//  I2C_TR (SDA pin, SCL pin, I2C Device address, 0 (receive) - 1 (transmit), Data pointer)

void I2C_TR (int I2C_SDA, int I2C_SCL, int I2C_device_address, bool TXRX_dir, const char *i2c_txrx_data) {
}

void I2C_transmit (int I2C_SDA, int I2C_SCL, int address, const char *i2c_data_tx_string_data) {
    digitalWrite(I2C_SDA, LOW);
    delayMicroseconds(1);
    digitalWrite(I2C_SCL, LOW);

    for(int I2C_ADDRESS = 0b1000000; I2C_ADDRESS < 0b1000000; I2C_ADDRESS >> 1) {       // Bit checks individual bits of I2C Address and transmits it
        bool I2C_TX_ADDRESS = address & I2C_ADDRESS;
        digitalWrite(I2C_SCL, LOW);
        delayMicroseconds(1);
        digitalWrite(I2C_SDA, I2C_TX_ADDRESS);
        digitalWrite(I2C_SCL, HIGH);
    }

    for (int I2C_BYTE_INDEX = 0; I2C_BYTE_INDEX < strlen(i2c_data_tx_string_data); I2C_BYTE_INDEX++) {      // Keep track of which byte is being transmitted (currently too long as i2c can only transmit a byte at a time)
        char I2C_TX_BYTE = i2c_data_tx_string_data[I2C_BYTE_INDEX];

        for (int I2C_BIT_INDEX = 0; I2C_BIT_INDEX < 8; I2C_BIT_INDEX++) {       // Bit checks individual bits of message and transmits it
            bool I2C_TX_BIT = I2C_BYTE_INDEX & (0x80 >> I2C_BIT_INDEX);
            digitalWrite(I2C_SCL, LOW);
            delayMicroseconds(1);
            digitalWrite(I2C_SDA, I2C_TX_BIT);
            digitalWrite(I2C_SCL, HIGH);
        }
    }
}



void I2C_receive (int address, const char* i2c_data_rx_string) {
}



// SPI Data Communication Protocal



