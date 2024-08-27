#include <Arduino.h>
#include <Display.h>
#include <ImageTest.h>

HUB75 Panels;
Compose compose;
Shader Modify;

float colourcycle = 0;
float colourvelocity = 0;
int velocitycycle = 0;
float colourcycle2 = 0;
float colourvelocity2 = 0;
int velocitycycle2 = 0;

const int amplitude = 3;   // Peak amplitude (0-255)
const float frequency = 0.25;    // Frequency in Hz
const float frequency2 = 0.3;
const int offset = 10;      // Offset (shifts the wave up or down)

void setup() {
    Serial.begin(9600);
    Panels.PanelCount(2);
    Panels.Buffer(5);
    Panels.Initialise();
    for(size_t i = 0; i <= 500; i++){
        compose.BlendtoLayer(Panels.Array, 128, 32, CrowLogo, 38, 28, 5, 3, 1);
        Panels.Print("CREATED BY XCR-1793", 0xf800, 5, 4, 42, 10);
            for(size_t k = 0; k <= 127; k++){
            colourcycle2 = colourvelocity2 + i;
            for(size_t j = 0; j <= 31; j++){
                if(Panels.Array[k+(j*128)]==0xf800){Panels.DrawPixel(k, j+1, Modify.HueReturnColour(colourcycle2));}
                colourcycle2++;
            }
        }
        Panels.drive_HUB75();
        Panels.Clear();
    }
}

int fontsize = 0;
long increment = 1;
void loop() {
    // esp_task_wdt_reset();
    unsigned long currentTime = millis();
    compose.BlendtoLayer(Panels.Array, 128, 32, sprite2, 24, 13, 16 + amplitude * sin(2 * PI * frequency2 * currentTime / 1000.0), 10 - amplitude * sin(2 * PI * frequency * currentTime / 1000.0), 0);
    compose.BlendtoLayer(Panels.Array, 128, 32, sprite3, 24, 13, 64 + 24 - amplitude * sin(2 * PI * frequency2 * currentTime / 1000.0), 10 -amplitude * sin(2 * PI * frequency * currentTime / 1000.0), 0);
    for(size_t i = 0; i <= 63; i++){
        colourcycle = colourvelocity + i;
        for(size_t j = 0; j <= 31; j++){
            if(Panels.Array[i+(j*128)]){Panels.DrawPixel(i, j+1, Modify.HueReturnColour(colourcycle));}
            colourcycle++;
        }
    }
    for(size_t i = 0; i <= 63; i++){
        colourcycle2 = colourvelocity2 - i;
        for(size_t j = 0; j <= 31; j++){
            if(Panels.Array[64+i+(j*128)]){Panels.DrawPixel(64+i, j+1, Modify.HueReturnColour(colourcycle2));}
            colourcycle2++;
        }
    }
    Panels.drive_HUB75();
    Panels.Clear();
    //colourcycle = colourvelocity;
    velocitycycle++;
    if(!(velocitycycle%5)){
        colourvelocity++;
    }
    velocitycycle2++;
    if(!(velocitycycle2%5)){
        colourvelocity2++;
    }

}
