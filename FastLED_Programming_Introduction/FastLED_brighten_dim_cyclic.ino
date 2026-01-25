#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    3
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
int brightness = 100;
bool dim = 1;
CRGB leds[NUM_LEDS];
int readLight = 0;
void setup() {
  Serial.begin(115200);
  delay(300);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);

  // Turn ALL LEDs on (red)
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
    Serial.println(i);
  }

  FastLED.show();
  pinMode(A0, INPUT);
}

void loop() {
  delay(500);
  Serial.println("hi!");
  int readLight = analogRead(A0);
  Serial.print("Light: ");
  Serial.println(readLight);
  FastLED.setBrightness(brightness);
  FastLED.show();
  if (brightness==0){
    dim=0;
  }
  if (brightness==100){
    dim=1;
  }
  if (dim==1){
    brightness-=5;
  } else {
    brightness+=5;
  }
  
}
