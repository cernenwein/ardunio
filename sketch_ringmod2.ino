
#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
#include <avr/power.h>
#endif

#define PIN 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN);
// http://learn.adafruit.com/downloads/pdf/adafruit-neopixel-uberguide.pdf

uint8_t  mode   = 1, // Current animation effect
         offset = 0, // Position of spinny eyes
         offset2 = 0; // Position of spinny eyes
uint32_t color  = 0xFF0000; // Start red
uint32_t prevTime;
uint8_t  i;
uint32_t t;


void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  strip.begin();
  strip.setBrightness(255); // 1/3 brightness
  strip.show();
}

void loop() {
  colorWipe(strip.Color(0, 0, 0), 125); // Black
  //  colorWipe(strip.Color(64, 0, 0), 100); // Red
  //  colorWipe(strip.Color(0, 64, 0), 100); // Green
  //  colorWipe(strip.Color(0, 0, 64), 100); // Blue
  //  colorWave(75);
  //  colorWipe(strip.Color(0,0,0), 100); // Black
    rainbow(15);
  //  colorWipe(strip.Color(0,0,0), 100); // Black
  //  rainbowCycle(15);
  //  colorWipe(strip.Color(0,0,0), 100); // Black
  //  colorWave(30);
  spinyWheels(50);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

/**
 * Scale a value returned from a trig function to a byte value.
 * [-1, +1] -> [0, 254]
 * Note that we ignore the possible value of 255, for efficiency,
 * and because nobody will be able to differentiate between the
 * brightness levels of 254 and 255.
 */
byte trigScale(float val) {
  val += 1.0; // move range to [0.0, 2.0]
  val *= 127.0; // move range to [0.0, 254.0]

  return int(val) & 255;
}

/**
 * Map an integer so that [0, striplength] -> [0, 2PI]
 */
float map2PI(int i) {
  return PI * 2.0 * float(i) / float(strip.numPixels());
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}



// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

/**
 *      ^   ^   ^
 * ~~~~~ ColorWave ~~~~~
 *        V   V   V
 */
void colorWave(uint8_t wait) {
  int i, j, stripsize, cycle;
  float ang, rsin, gsin, bsin, offset;

  static int tick = 0;

  stripsize = strip.numPixels();
  cycle = stripsize * 25; // times around the circle...

  while (++tick % cycle) {
    offset = map2PI(tick);

    for (i = 0; i < stripsize; i++) {
      ang = map2PI(i) - offset;
      rsin = sin(ang);
      gsin = sin(2.0 * ang / 3.0 + map2PI(int(stripsize / 6)));
      bsin = sin(4.0 * ang / 5.0 + map2PI(int(stripsize / 3)));
      strip.setPixelColor(i, strip.Color(trigScale(rsin), trigScale(gsin), trigScale(bsin)));
    }

    strip.show();
    delay(wait);
  }
}

void spinyWheels(uint8_t wait) {
  int i, j, stripsize, cycle;

  static int tick = 0;
  offset = 0;
  stripsize = strip.numPixels();
  cycle = stripsize * 25; // times around the circle...

  while (++tick % cycle) {


    for (i = 0; i < stripsize; i++) {
      uint32_t c = 0;
      if (((offset + i) & 7) < 2) c = color; // 4 pixels on...
      strip.setPixelColor(            i, c); // First eye
      strip.setPixelColor(stripsize - i, c); // Second eye (flipped)
    }
    strip.show();
    offset++;
    delay(wait);

    t = millis();
    if ((t - prevTime) > 8000) {     // Every 8 seconds...
      mode++;                        // Next mode
      if (mode > 1) {                // End of modes?
        mode = 0;                    // Start modes over
        color >>= 8;                 // Next color R->G->B
        if (!color) color = 0xffae00; // Reset to red
      }
      for (i = 0; i < 32; i++) strip.setPixelColor(i, 0);
      prevTime = t;

    }
  }
}

