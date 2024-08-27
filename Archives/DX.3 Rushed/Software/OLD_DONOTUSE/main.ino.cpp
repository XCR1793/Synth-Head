# 1 "C:\\Users\\micha\\AppData\\Local\\Temp\\tmp9sqml4hz"
#include <Arduino.h>
# 1 "C:/Users/micha/Desktop/Projects & Portfolios/ARCOS/src/main.ino"
#include <Arduino.h>



#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif



#define LED_PIN 14


#define LED_COUNT 200


#define BRIGHTNESS 50


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
# 36 "C:/Users/micha/Desktop/Projects & Portfolios/ARCOS/src/main.ino"
void colorWipe(uint32_t color, int wait);
void whiteOverRainbow(int whiteSpeed, int whiteLength);
void pulseWhite(uint8_t wait);
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops);
void setup();
void loop();
#line 36 "C:/Users/micha/Desktop/Projects & Portfolios/ARCOS/src/main.ino"
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;
  int loops = 3;
  int loopNum = 0;
  uint32_t lastTime = millis();
  uint32_t firstPixelHue = 0;

  for(;;) {
    for(int i=0; i<strip.numPixels(); i++) {
      if(((i >= tail) && (i <= head)) ||
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255));
      } else {
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show();



    firstPixelHue += 40;

    if((millis() - lastTime) > whiteSpeed) {
      if(++head >= strip.numPixels()) {
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {
        tail = 0;
      }
      lastTime = millis();
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) {

    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) {
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;





  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<strip.numPixels(); i++) {




      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());





      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    strip.show();
    delay(wait);

    if(firstPixelHue < 65536) {
      if(fadeVal < fadeMax) fadeVal++;
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) {
      if(fadeVal > 0) fadeVal--;
    } else {
      fadeVal = fadeMax;
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) {

      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
    delay(1000);
    for(int j=255; j>=0; j--) {
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
  }

  delay(500);
}

void setup() {


#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif


  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);
}

void loop() {

  colorWipe(strip.Color(255, 0, 0) , 50);
  colorWipe(strip.Color( 0, 255, 0) , 50);
  colorWipe(strip.Color( 0, 0, 255) , 50);
  colorWipe(strip.Color( 0, 0, 0, 255), 50);

  whiteOverRainbow(75, 5);

  pulseWhite(5);

  rainbowFade2White(3, 3, 1);
}