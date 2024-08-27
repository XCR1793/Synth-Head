#include <Arduino.h>
#include <driver/adc.h>
#include <Display.h>
#include <ImageTest.h>

HUB75 Panels;

// Around khz without Serial.begin using a blank drive
// Aronud 125.92hz with Serial.begin using a blink drive
void setup(){
    // Serial.begin(115200);
    Panels.Buffer(3);
    Panels.Initialise();
    pinMode(23, OUTPUT);
}

void loop(){
    for(size_t i = 0; i <= 10000; i ++){
        // REG_WRITE(GPIO_OUT_W1TS_REG, (1 << 23));  // Set the pin
        Panels.drive_HUB75();
        Panels.Clear();
        // REG_WRITE(GPIO_OUT_W1TC_REG, (1 << 23));  // Clear the pin
        Panels.drive_HUB75();
        Panels.Clear();
    }
}


// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <driver/gpio.h>
// #include <driver/dma.h>
// #include <soc/gpio_struct.h>

// #define DMA_CHANNEL 1
// #define BUFFER_SIZE 100

// void configureDMA() {
//     // Initialize DMA controller
//     dma_init();

//     // Configure DMA channel
//     dma_channel_config_t dmaConfig = {
//         .channel = DMA_CHANNEL,
//         .direction = 1,  // Memory to memory
//         .src_inc = 1,
//         .dst_inc = 1,
//         .src_data = NULL,  // Source data not needed for GPIO toggle
//         .dst_data = &GPIO.out1_w1ts.val,  // Destination is GPIO output register
//         .total_len = BUFFER_SIZE,
//     };

//     // Enable DMA channel
//     dma_channel_enable(&dmaConfig);
// }

// void app_main() {
//     // Configure GPIO pin 0 as an output
//     gpio_config_t io_conf = {
//         .pin_bit_mask = (1ULL << GPIO_NUM_0),
//         .mode = GPIO_MODE_OUTPUT,
//     };
//     gpio_config(&io_conf);

//     // Configure DMA
//     configureDMA();

//     while (1) {
//         // Start DMA transfer to toggle GPIO pin 0
//         dma_start(DMA_CHANNEL);

//         // Wait for DMA transfer to complete
//         while (dma_busy(DMA_CHANNEL)) {
//             // Optionally, you can perform other tasks here
//             vTaskDelay(1 / portTICK_PERIOD_MS);
//         }
//     }
// }


