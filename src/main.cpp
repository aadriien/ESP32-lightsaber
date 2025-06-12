// Lightsaber
// Animated lightsaber activation and deactivation, using ESP32.
// by Adrien Lynch

#include <FastLED.h>
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


void printColor(const CRGB& color);
float getHueFromRGB(const CRGB& color);
const char* getNuancedColorName(const CRGB& color);
bool getColorFromJoystick(int joyX, int joyY, CRGB &outColor);
bool isJoystickOutsideDeadzone(int joyX, int joyY);
void checkClicked(int joyX, int joyY);
void activateLightsaber(CRGB color);
void deactivateLightsaber();



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
                // Serial.println("Color updated");

                // Print basic color name for sound triggers
                Serial.print("Play COLOR: ");
                Serial.println(getNuancedColorName(currentColor));
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


float getHueFromRGB(const CRGB& color) {
    float r = color.r / 255.0;
    float g = color.g / 255.0;
    float b = color.b / 255.0;

    float maxVal = max(r, max(g, b));
    float minVal = min(r, min(g, b));
    float delta = maxVal - minVal;

    if (delta < 0.00001) return 0;  // gray/white/black (no hue)

    float hue = 0;

    if (maxVal == r) {
        hue = 60.0 * fmod(((g - b) / delta), 6);
    } else if (maxVal == g) {
        hue = 60.0 * (((b - r) / delta) + 2);
    } else if (maxVal == b) {
        hue = 60.0 * (((r - g) / delta) + 4);
    }

    if (hue < 0) hue += 360;

    return hue;
}


const char* getNuancedColorName(const CRGB& color) {
    float hue = getHueFromRGB(color);

    float saturation = 
        (float)max(color.r, max(color.g, color.b)) 
        - 
        (float)min(color.r, min(color.g, color.b));
    saturation /= 255.0;

    float brightness = max(color.r, max(color.g, color.b)) / 255.0;

    // Handle black/white/gray first
    if (brightness < 0.1) return "Black";
    if (saturation < 0.1 && brightness > 0.9) return "White";
    if (saturation < 0.1) return "Gray";

    // Hue ranges with more nuance:
    if (hue >= 0 && hue < 15) return "Red";
    if (hue >= 15 && hue < 45) return "Orange";
    if (hue >= 45 && hue < 70) return "Yellow";
    if (hue >= 70 && hue < 150) return "Green";
    if (hue >= 150 && hue < 190) return "Cyan";
    if (hue >= 190 && hue < 260) return "Blue";
    if (hue >= 260 && hue < 320) return "Purple";
    if (hue >= 320 && hue < 345) return "Magenta";
    if (hue >= 345 && hue <= 360) return "Red";

    return "Unknown";
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



