// Lightsaber
// Animated lightsaber activation and deactivation, using ESP32.
// by Adrien Lynch

#include "FastLED.h"
#include <math.h>  // Needed for atan2 and sqrt in color selection

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    5
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    60
#define BRIGHTNESS  255

#define JOY_X_PIN   39
#define JOY_Y_PIN   36
#define JOY_BUTTON_PIN 17

#define DEADZONE_RADIUS 400  // Joystick central zone (for color lock check)

CRGB leds[NUM_LEDS];
CRGB currentColor = CRGB::Blue;  // Default starting color

bool isActivated = false;
bool lastButtonState = HIGH;
bool colorLocked = false;


void setup() {
  Serial.begin(9600);
  delay(3000);  // Startup delay

  // Prep LEDs for output
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(JOY_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int joyX = analogRead(JOY_X_PIN);
  int joyY = analogRead(JOY_Y_PIN);
  
  checkClicked(joyX, joyY);

  // Read joystick changes & update color while lightsaber is ON
  if (isActivated) {
    if (!colorLocked) {
      CRGB newColor;
      if (getColorFromJoystick(joyX, joyY, newColor)) {
        currentColor = newColor;
        Serial.println("Color updated");
      }
    }

    fill_solid(leds, NUM_LEDS, currentColor);
    FastLED.show();
  }

  delay(50);  // Smooth operation
}


void printColor(const CRGB& color) {
  Serial.print("Activated Color - R: ");
  Serial.print(color.r);
  Serial.print(" G: ");
  Serial.print(color.g);
  Serial.print(" B: ");
  Serial.println(color.b);
}


bool getColorFromJoystick(int joyX, int joyY, CRGB &outColor) {
  int centerX = 2048;
  int centerY = 2048;

  int dx = joyX - centerX;
  int dy = centerY - joyY;

  float distance = sqrt(dx * dx + dy * dy);
  float maxDistance = 2048.0;
  float normDist = distance / maxDistance;

  // Only update color if joystick button pushed enough
  if (normDist < 0.2) {
    return false;  // Stay with last color
  }

  // Calculate color wheel presentation based on joystick X/Y
  float angle = atan2(dy, dx);
  if (angle < 0) angle += 2 * PI;

  uint8_t hue = map((int)(angle * 180 / PI), 0, 360, 0, 255);
  uint8_t saturation = constrain(normDist * 255, 0, 255);
  uint8_t brightness = 255;

  outColor = CHSV(hue, saturation, brightness);
  return true;
}


bool isJoystickOutsideDeadzone(int joyX, int joyY) {
  // Ensure we don't get stuck in default white color while joystick centered
  int centerX = 2048;
  int centerY = 2048;
  
  int dx = joyX - centerX;
  int dy = joyY - centerY;
  
  int distSq = dx*dx + dy*dy;
  return distSq > (DEADZONE_RADIUS * DEADZONE_RADIUS);
}


void checkClicked(int joyX, int joyY) {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(JOY_BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    if (!isActivated) {
      // Turn on lightsaber
      activateLightsaber(currentColor);
      isActivated = true;
      colorLocked = false;
      
      Serial.println("Lightsaber activated!");
      printColor(currentColor);
    } 
    else {
      // Lightsaber is on: toggle color lock if joystick pushed
      if (isJoystickOutsideDeadzone(joyX, joyY)) {
        colorLocked = !colorLocked;
        
        Serial.println(colorLocked ? "Color locked!" : "Color unlocked!");
        if (colorLocked) printColor(currentColor);
        
      } 
      else {
        // Otherwise, turn off lightsaber
        deactivateLightsaber();
        isActivated = false;
        colorLocked = false;
        
        Serial.println("Lightsaber deactivated!");
      }
    }
  }
  lastButtonState = currentButtonState;
}


void activateLightsaber(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
    FastLED.show();
    delay(20);
  }
}


void deactivateLightsaber() {
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(20);
  }
}
