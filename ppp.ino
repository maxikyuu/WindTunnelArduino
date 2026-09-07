#include <Servo.h>
#include <FastLED.h>

#define LED_PIN     6    // The Arduino digital output pin connected to DIN
#define NUM_LEDS    6   // Change this to the exact number of LEDs on your strip
#define BRIGHTNESS  64   // Set overall brightness (0 to 255)

CRGB leds[NUM_LEDS];     // Define the array of LEDs

int buttonLED = 2;
int buttonServo = 3;
int servoPin = 7;
int servoPos = 0;
Servo servo;

// Constants
const long LEDInterval = 1000;       // Frequency at which button checks are polled
// State Variables
unsigned long currentMs = 0;            // Current time in ms
unsigned long previousButtonMs = 0;     // Time since previous button check in ms

int LEDstate=0;
int state=0;
int buttonLEDNew;
int buttonValue;
int buttonLEDOld=1;

int buttonServoNew;
int buttonServoOld=1;
int dt=100;

// Toggles between different LED states (0 = white, 1 = red, 2 = green)
void LEDStateToggle() {
  if (buttonLEDOld==1 && buttonLEDNew==0) {
    LEDstate++;
    if (LEDstate > 2) {
      LEDstate = 0;
    }
  }
  // Serial.println(LEDstate);
  buttonLEDOld = buttonLEDNew; 
}

void checkLEDButton() {
  buttonValue = digitalRead(buttonLED);
  buttonLEDNew = buttonValue;
}

void checkServoButton() {
  buttonServoNew = digitalRead(buttonServo);
  if(buttonServoOld==1 && buttonServoNew==0){
    state++;
    if (state > 3) {
      state = 0;
    }
    switch(state){
      case(0):
        servoPos = 0;
        servo.write(servoPos);
        Serial.println("Zero degrees");
        break;
      case(1):
        servoPos = 15;
        servo.write(servoPos);
        Serial.println("15 degrees");
        break;
      case(2):
        servoPos = 30;
        servo.write(servoPos);
        Serial.println("30 degrees");
        break;
      case(3):
        servoPos = 160;
        servo.write(servoPos);
        Serial.println("160 degrees");
        break;
    }
  }
  buttonServoOld=buttonServoNew;
}

void setup() {
  //put your setup code here, to run once:
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(buttonServo, INPUT);
  pinMode(buttonLED, INPUT);
  servo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //checkButton();
  checkLEDButton();
  checkServoButton();
  LEDStateToggle();
  currentMs = millis();


  // if (currentMs - previousButtonMs >= LEDInterval) {
  //   fill_solid(leds, NUM_LEDS, CRGB::White);
  //   FastLED.show();
  //   previousButtonMs = currentMs;
  // }
  if (currentMs - previousButtonMs >= LEDInterval) {
    switch(LEDstate){
      case(0):
        fill_solid(leds, NUM_LEDS, CRGB::White);
        break;
      case(1):
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        break;
      case(2):
        fill_solid(leds, NUM_LEDS, CRGB::Green);
        
        break;
    }
    previousButtonMs = currentMs;
    FastLED.show();
  }
}
