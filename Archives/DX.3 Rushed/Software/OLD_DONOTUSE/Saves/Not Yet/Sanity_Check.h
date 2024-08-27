// Work in progress

#pragma once



// I2C Out Of Range Detection
#ifdef I2C_Count_Available
    #if I2C_Count_Available == 0
        #ifdef I2C_1 || I2C_2 || I2C_3 || I2C_4 || I2C_5 || I2C_6 || I2C_7
            #error "I2C Out Of Range (Over 0)"
        #endif
    #endif

    #if I2C_Count_Available == 1
        #ifdef I2C_2 || I2C_3 || I2C_4 || I2C_5 || I2C_6 || I2C_7
            #error "I2C Out Of Range (Over 1)"
        #endif
    #endif

    #if I2C_Count_Available == 2
        #ifdef I2C_3 || I2C_4 || I2C_5 || I2C_6 || I2C_7
            #error "I2C Out Of Range (Over 2)"
        #endif
    #endif

    #if I2C_Count_Available == 3
        #ifdef I2C_4 || I2C_5 || I2C_6 || I2C_7
            #error "I2C Out Of Range (Over 3)"
        #endif
    #endif

    #if I2C_Count_Available == 4
        #ifdef I2C_5 || I2C_6 || I2C_7
            #error "I2C Out Of Range (Over 4)"
        #endif
    #endif
#endif
