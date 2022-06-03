#include <FastLED.h> // Manage Libraries -> Install FastLED

//#define DEBUG
#define SOUND

#define LED_PIN     2
#define NUM_LEDS    9
#define MOTION_PIN  4
#define SOUND_PIN   A6
#define BUZZER_PIN  11
#define BUTTON_PIN  10 
#define SOUND_SAMPLE_WINDOW 50
#define MODIFIER 150

#ifdef SOUND
  #include "BrailleSpeak.h"
  BrailleSpeak brailleSpeak(BUZZER_PIN);
#endif


unsigned int sleep_timeout;
unsigned int sample;
CRGB leds[NUM_LEDS];
int breath;
boolean breathe_in;
unsigned long breath_pause;
unsigned long last_movement;
unsigned long enable_sleep;


//#include "LEDHelper.h"
//LEDHelper ledhelper(LED_PIN, leds);
 
void setup() 
{
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);
//  #ifdef DEBUG
    Serial.begin(9600);
//  #endif

  last_movement = 0;
  sleep_timeout = 60;
  breath = 0;
  breathe_in = true;
  breath_pause = millis();
  enable_sleep = false;
  #ifdef SOUND
    brailleSpeak.speak("hi");
  #endif
}

char getRandomLetter()
{
   byte randomValue = random(0, 26);
   char letter = randomValue + 'a';
//   if(randomValue > 26)
//      letter = (randomValue - 26) + '0';
   Serial.println(letter);
   return letter;
}
 
 
void loop() 
{
  
  int motion = digitalRead(MOTION_PIN);
  int touch = digitalRead(BUTTON_PIN);
  if (motion == HIGH) {
      last_movement = millis();
  }
  if (touch == HIGH) {
      #ifdef SOUND
        brailleSpeak.handleChar(getRandomLetter());
        delay(200);
      #endif
  }
  
  double sound = 0.0;
  breathe();
  if (enable_sleep && motion == LOW && last_movement <= millis_minus_timeout()) {
    // I'm asleep becauase i haven't felt you move for X seconds
    leds[0] = CRGB(0, 0, breath_sin());
    leds[1] = CRGB(0, 0, 0);
    leds[2] = CRGB(0, 0, 0);
//    delay(100);
  }
  else {
    // I'm awake and listening to noises
    sound = sample_sound();
    
    leds[0] = CRGB(0, 0, breath_sin());
    leds[1] = CRGB(0, 0, sound);
    leds[2] = CRGB(0, 0, sound);
  
    
  }

  #ifdef DEBUG
    Serial.print(touch * 100);
    Serial.print(',');
    Serial.print(motion * 100);
    Serial.print(',');
    Serial.println(sound);
//    Serial.println(last_movement);
//    Serial.println(millis());
//    Serial.println(sleep_timeout * 1000);
//    Serial.println(millis_minus_timeout());
  #endif
  
  FastLED.show();
}

int millis_minus_timeout() {
  if (millis() < sleep_timeout * 1000) return 0;
  return millis() - sleep_timeout * 1000;
}

double sample_sound() {
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  
  // collect data for 50 mS
  while (millis() - startMillis < SOUND_SAMPLE_WINDOW)
  {
    sample = analogRead(A6);
    
    if (sample < 1024)  // toss out spurious readings
    {
       if (sample > signalMax)
       {
          signalMax = sample;  // save just the max levels
       }
       else if (sample < signalMin)
       {
          signalMin = sample;  // save just the min levels
       }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  unsigned int modified = peakToPeak;
  if (modified < 100) modified = 0;
  else modified = modified - 100;
  
//  #ifdef DEBUG
//      Serial.print(sample);
//      Serial.print(',');
//      Serial.print(peakToPeak);
//      Serial.print(',');
//      Serial.println(modified);
//    #endif
  
  return modified;
}

int breath_sin() {
  return (sin(breath * 0.0174533) + 1) * 80 - 80;
}

int breathe() {
  int step = 3;
  int next;
  if (breath_pause > millis() - 2000) {
    return;
  }
  if (breathe_in == true) {
    next = breath = breath + step;
    if (breath > 80) {
      next = 80;
      breathe_in = false;
    }
  }
  else {
    next = breath = breath - step;
    if (breath < 0) {
      next = 0;
      breath_pause = millis();
      breathe_in = true;
    }
  }
  breath = next;
  return breath;
}


void led_set(int identifier, int r, int g, int b)
{
  leds[identifier] = CRGB(r, g, b);
  
}
