
#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
#include <avr/power.h>
#endif

#define PIN 0

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, PIN);
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
  pixels.setBrightness(3); // 1/3 brightness
  prevTime = millis();
}

void loop() {
  uint8_t  i;
  uint32_t t;

      for (i = 0; i < 16; i++) {
//        uint32_t c = 0;
//        if (((offset + i) & 7) < 2) c = color; // 4 pixels on...
        /*
         * pixels.setPixelColor(n, red, green, blue);
         * n is 0 based position
         * The next three arguments are the pixel color, 
         * expressed as red, green and blue brightness levels,
         * where 0 is dimmest (off) and 255 is maximum brightness
         * or define color as
         * uint32_t magenta = pixels.Color(255, 0, 255);
         */
        if ( (i % 2) == 0)
          pixels.setPixelColor(i, color + offset2); // First eye
        if ( (i % 2) == 1)
          pixels.setPixelColor(i, color - offset2); // First eye
        if ( (i % 4) == 0)
          pixels.setPixelColor(i, color); // First eye
        if ( (i % 4) == 0)
          pixels.setPixelColor(i, color); // First eye    
      }
      pixels.setPixelColor(offset2, 0xFF0000); // First eye   
      // setPixelColor() does not have an immediate effect on the LEDs. 
      // To “push” the color data to pixels, call show():
      pixels.show();
      offset++;
      delay(80);  // in miliseconds
      offset2++;
      color >>= 1; 
      
      if (!color) color = 0xFF0000; // Reset to red
      if (offset2 > 16) offset2 = 0;
  
}
