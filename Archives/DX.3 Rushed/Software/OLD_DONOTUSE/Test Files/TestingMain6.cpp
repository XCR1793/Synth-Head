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

void Idle(){
    Panels.Print("CREATED BY",  0x0801, 5, 4, 87, 18);
    Panels.Print("XCR-1793",    0x0801, 5, 4, 95, 26);

    Panels.Print("07/11/23", 0x0021, 7, 5, 4, 10);      // Use RTC
    Panels.Print("12:04 PM", 0x0021, 7, 5, 4, 20);      // Use RTC
}

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

void loop() {
    Idle();
    Panels.drive_HUB75();
    Panels.Clear();
}


