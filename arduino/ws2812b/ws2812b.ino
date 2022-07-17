#include <FastLED.h>

#define DATA_PIN    13
#define BRIGHTNESS  16
#define NUM_LEDS    640
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB


// Set serial for debug console (to the Serial Monitor, default speed 115200)
//
#define SerialMon Serial

// Le tableau pour les LED
//
CRGB leds[NUM_LEDS];

void setup() {

  SerialMon.begin(115200);
  
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // Allumer les LED
  //
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Blue;
  leds[3].r = 254;
  leds[3].g = 254;
  leds[3].b = 0;

  Serial.print("Boucle de 1000 show(), Nombre de LED ");
  Serial.println(NUM_LEDS);
  for (int i = 0; i < 1000; i++) {
    FastLED.show();
  }
  Serial.println("Fin");
}
