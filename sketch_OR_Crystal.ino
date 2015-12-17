#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
#include <avr/power.h>
#endif

#define PIN 0

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIN);
// http://learn.adafruit.com/downloads/pdf/adafruit-neopixel-uberguide.pdf

uint8_t  mode   = 1, // Current animation effect
         offset = 0, // Position of spinny eyes
         offset2 = 0; // Position of spinny eyes
uint32_t color  = 0xFF0000; // Start red
uint32_t prevTime;

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  //The overall brightness of all the LEDs can be adjusted using setBrightness(). This takes a single argument, a number in the range 0 (off) to 255 (max brightness). For example, to set a strip to 1/4 brightness:
  pixels.setBrightness(255); // 1/3 brightness
  prevTime = millis();
}

void loop() {
  uint8_t  i;
  uint32_t c;

    for (c = 5; c < 255; c+=5) {
      for (i = 0; i < 32; i++) {
        pixels.setPixelColor(i, 0, c, 0);
      }
      delay(10);  
      pixels.show();
    }

    //Pauses the program for the amount of time (in miliseconds) specified as parameter. (There are 1000 milliseconds in a second.)
    delay(600);  
 
    for (c = 255; c > 5; c-=5) {
      for (i = 0; i < 32; i++) {
        pixels.setPixelColor(i, 0, c, 0);
      }
      delay(10);  
      pixels.show();
    }
    
    delay(600);  
}
