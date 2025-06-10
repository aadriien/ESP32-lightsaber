// Lightsaber
// Animated lightsaber activation and deactivation, using ESP32.
// by Adrien Lynch

#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    5
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    60
#define BRIGHTNESS  255

// Define ESP32 analog pins for X and Y axes
#define JOY_X_PIN   39  // Joystick X-axis pin (VN)
#define JOY_Y_PIN   36  // Joystick Y-axis pin (VP)
#define JOY_BUTTON_PIN 2  // Joystick button pin (optional)


CRGB leds[NUM_LEDS];


void setup() {
  Serial.begin(9600);
  
  delay(3000); // 3 second delay for recovery
  
  // Tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);

  pinMode(JOY_BUTTON_PIN, INPUT_PULLUP); // Set joystick button as input
}

void loop() {
  // Read joystick position (analogRead returns 0 to 4095 on ESP32)
  int joyX = analogRead(JOY_X_PIN);  // Read X-axis (VN)
  int joyY = analogRead(JOY_Y_PIN);  // Read Y-axis (VP)
  
  // Map joystick position to color
  CRGB currentColor = getColorFromJoystick(joyX, joyY);


  Serial.print("joyX: ");
  Serial.print(joyX);
  
  Serial.print(" joyY: ");
  Serial.print(joyY);
  
  Serial.print("  Selected color: ");
  Serial.print("R: ");
  Serial.print(currentColor.r);  // Red component
  Serial.print(" G: ");
  Serial.print(currentColor.g);  // Green component
  Serial.print(" B: ");
  Serial.println(currentColor.b);  // Blue component
  

  // Activate lightsaber with selected color
  activateLightsaber(currentColor);
  deactivateLightsaber();
  
  // Check if button is pressed (optional: to activate/deactivate lightsaber)
//  if (digitalRead(JOY_BUTTON_PIN) == LOW) {
//    // You can add functionality here to toggle lightsaber ON/OFF if needed
//  }

  delay(100);  // Small delay for smooth operation
}

CRGB getColorFromJoystick(int joyX, int joyY) {
  // Map joystick positions to colors
  if (joyY < 1500) {  // Joystick is pushed up (Blue)
    return CRGB::Blue;
  } 
  else if (joyY > 3000) {  // Joystick is pushed down (Red)
    return CRGB::Red;
  }
  else if (joyX < 1500) {  // Joystick is pushed left (Green)
    return CRGB::Green;
  }
  else if (joyX > 3000) {  // Joystick is pushed right (Yellow)
    return CRGB::Yellow;
  }

  // Default color (Black if no joystick movement)
  return CRGB::Black;
}

void activateLightsaber(CRGB color) {
  // Turn ON each LED in addressable strip sequentially with selected color
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
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
