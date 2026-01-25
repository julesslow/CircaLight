#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    3
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

// HC-SR04 pins (match your diagram)
#define TRIG_PIN 6
#define ECHO_PIN 7

int brightness = 100;
bool dim = 1;
bool here = 1;
int wait_count = 0; //count until they check if the door has been opened again
int max_count = 50;
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  delay(300);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);

  // Turn ALL LEDs on (white)
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
    Serial.println(i);
  }
  FastLED.show();

  pinMode(A0, INPUT);

  // HC-SR04 setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

// returns distance in cm, or -1 if timed out
float readDistanceCM() {
  // Send a 10us pulse to TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pulse width (timeout ~30ms ≈ 5m range)
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return -1; // timeout

  // Speed of sound ≈ 0.0343 cm/us
  float distance = (duration * 0.0343f) / 2.0f;
  return distance;
}

void loop() {
  delay(500);

  Serial.println("hi!");

  int readLight = analogRead(A0);
  Serial.print("Light: ");
  Serial.println(readLight);

  if (wait_count==0){ // only check distance after the wait timer has finished
    float dist = readDistanceCM();
    Serial.print("Distance (cm): ");
    if (dist > 15){
      here = !here;
      wait_count++;
    }
    if (dist < 0) Serial.println("timeout/no echo");
    else Serial.println(dist, 2);
  } else if (wait_count==max_count){
    wait_count=0; // reset and end wait timer
  } else{
    wait_count++;
  }
    
  FastLED.setBrightness(brightness);
  FastLED.show();

  if (brightness == 0)  dim = 0;
  if (brightness == 100) dim = 1;

  if (dim == 1) brightness -= 5;
  else          brightness += 5;
}
