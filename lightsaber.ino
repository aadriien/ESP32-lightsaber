#include "FastLED.h"

// Lightsaber
// Animated lightsaber activation and deactivation, using ESP32.
// by Adrien Lynch

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    5
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    60
#define BRIGHTNESS  255

CRGB leds[NUM_LEDS];


void setup() {
  delay(3000); // 3 second delay for recovery
  
  // Tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);
}


void loop() {
  activateLightsaber();
  delay(800);
  deactivateLightsaber();

}


void activateLightsaber() {
  // Turn ON each LED in addressable strip sequentially
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(80);
  }
}


void deactivateLightsaber() {
  // Turn OFF each LED in addressable strip sequentially (reversed)
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(80);
  }
}
