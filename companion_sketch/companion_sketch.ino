#include <FastLED.h> // Manage Libraries -> Install FastLED

#define DEBUG
#define SOUND

#define MOTION_PIN  4
#define MIC_PIN   A6
#define BUZZER_PIN  11
#define BUTTON_PIN  10

#define SLEEP_TIMEOUT   60000 // in milliseconds (seconds * 1000)
#define ENABLE_SLEEP    0

#ifdef SOUND
  #include "BrailleSpeak.h" 
  BrailleSpeak brailleSpeak(BUZZER_PIN);
#endif

unsigned long last_movement; // Last time motion was detected

unsigned long touch_start; // start of last touch
unsigned long touch_end; // end of last touch


#include "ledhelper.h" // SETUP: configure #defines here
LEDHelper ledHelper;

#include "listener.h"
Listener listener(MIC_PIN);
 
void setup() 
{
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  #ifdef DEBUG
    Serial.begin(9600);
  #endif

  last_movement = 0;
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
//   Serial.println(letter);
   return letter;
}
 
 
void loop() 
{
  if (digitalRead(MOTION_PIN) == HIGH) {
      last_movement = millis();
  }

  // Track button presses and length
  if (digitalRead(BUTTON_PIN) == HIGH) {
      if (touch_end != NULL) 
      {
        touch_start = millis();
        touch_end = NULL;
      }
      #ifdef SOUND
        brailleSpeak.handleChar(getRandomLetter());
        delay(200);
      #endif
  }
  else if (touch_end == NULL) 
  {
    touch_end = millis();
  }
  
  double sound = 0.0;
  ledHelper.breathe();
  if (ENABLE_SLEEP && last_movement <= millis_minus_timeout()) {
    // I'm asleep becauase i haven't felt you move for SLEEP_TIMEOUT milliseconds
    ledHelper.setEye(0, 0, 0);
  }
  else {
    // I'm awake and listening to noises
    sound = listener.sample_sound();
    ledHelper.setEye(0, 0, sound);
  }
  ledHelper.set(0, 0, 0, ledHelper.breath_sin());
  
  FastLED.show();
}

int millis_minus_timeout() {
  if (millis() < SLEEP_TIMEOUT) return 0;
  return millis() - SLEEP_TIMEOUT;
}
