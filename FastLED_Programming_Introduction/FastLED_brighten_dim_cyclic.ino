#include <FastLED.h>
#include <math.h>


#define LED_PIN     5
#define NUM_LEDS    3
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB


// HC-SR04 pins (match your diagram)
#define TRIG_PIN 6
#define ECHO_PIN 7
#define LIGHT_SENSOR 0


int brightness = 50;
bool here = 1;
bool opened = 0;
int wait_count = 0; //count until they check if the door has been opened again
int max_count = 5;
CRGB leds[NUM_LEDS];


void setup() {
 Serial.begin(9600);
 delay(300);


 FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
 FastLED.setBrightness(brightness);


 // Turn ALL LEDs on (white)
 for (int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CRGB(90, 35, 0);
   Serial.println(i);
 }
 FastLED.show();


 pinMode(LIGHT_SENSOR, INPUT);


 // HC-SR04 setup
 pinMode(TRIG_PIN, OUTPUT);//TODO: uncomment
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



 if (wait_count==0){ // only check distance after the wait timer has finished //TODO: uncomment
   float dist = readDistanceCM();
   Serial.print("Distance (cm): ");
   if (dist > 15){
     opened = 1;
     Serial.println("Opened");
   } else if (opened==1){
     here = !here;
     opened = 0;
     wait_count++;
     Serial.println("Closed");
   }
   if (dist < 0) Serial.println("timeout/no echo");
   else Serial.println(dist, 2);
 } else if (wait_count==max_count){
   wait_count=0; // reset and end wait timer
 } else{
   wait_count++;
 }
 //Turn lights on if they are here - only want them to turn off the lights when they want to.


 if (here==1){
   int readLight = analogRead(LIGHT_SENSOR);
   Serial.print("Light: ");
   Serial.println(readLight);
   brightness = 100*(readLight*(1/1023))*(readLight*(1/1023)); // translating it to the brightness range of 0 to 100, from the photoresistor's range of 0 to 1023
   brightness = readLight*(90/1023);
   if (brightness==0){
     brightness = 5;
   }

   // brightness ranges
   // 900+ -> 90
   // 800-900 -> 80
   // 700-800 -> 70
   // 600-700 -> 60
   // 550-600 -> 50
   // 500-550 -> 40
   // 450-500 -> 30
   // 350-400 -> 20
   // 300-350 -> 10
   // 0-300 -> 0
   if (readLight>900){
    brightness=90;
   } else if (readLight>800){
    brightness=80;
   } else if (readLight>700){
    brightness=70;
   } else if (readLight>600){
    brightness=60;
   } else if (readLight>550){
    brightness=50;
   } else if (readLight>500){
    brightness=40;
   } else if (readLight>450){
    brightness=30;
   } else if (readLight>400){
    brightness=20;
   } else if (readLight>200){
    brightness=10;
   } else{
    brightness=0;
   }
   Serial.println(brightness);
   FastLED.setBrightness(brightness);
 } else{
   FastLED.setBrightness(0);
 }
 FastLED.show();
}

