#include <FastLED.h> // Manage Libraries -> Install FastLED

//#define DEBUG
#define SOUND

#define MOTION_PIN        4
#define MIC_PIN           A6
#ifdef SOUND
  #define BUZZER_PIN        11
#endif
#define BUTTON_PIN        3

#define SLEEP_TIMEOUT     60000 // in milliseconds (seconds * 1000)
#define ENABLE_SLEEP      0

#define MODE_LISTEN       0
#define MODE_KNOCKBACK    1
#define MODE_SECURITY     2
#define MODE_SIZE         3

#ifdef SOUND
#include "BrailleSpeak.h"
BrailleSpeak brailleSpeak(BUZZER_PIN);
#endif

unsigned long last_movement; // Last time motion was detected

unsigned long touch_start; // start of last touch

byte mode; // Which mode should we be running?

int ignore_motion = 0;

#include "ledhelper.h" // SETUP: configure #defines here
LEDHelper ledHelper;

#include "listener.h"
Listener listener(MIC_PIN);

#include "knockback.h"
#ifdef SOUND
  KnockBack knockback(MIC_PIN, BUZZER_PIN, &ledHelper);
#else
  KnockBack knockback(MIC_PIN, NULL, &ledHelper);
#endif

void setup()
{
#ifdef SOUND  
  pinMode(BUZZER_PIN, OUTPUT);
#endif
  pinMode(MOTION_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
#ifdef DEBUG
  Serial.begin(9600);
#endif

  last_movement = 0;
#ifdef SOUND
  brailleSpeak.speak("hi");
#endif

  mode = MODE_LISTEN;
  show_mode();
//  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), log_press, RISING); // Doesn't work
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), log_release, FALLING);
}

void log_press()
{
  // Doesn't work!
  Serial.println("press");
  touch_start = millis();
}
void log_release()
{
  unsigned long touch_end = millis();
  // Track button presses and length

//  Serial.println("release");
  ledHelper.setEye(0, 0, 0);
  FastLED.show();

  if (mode == MODE_LISTEN) {
    ledHelper.setEye(0, 10, 0);
#ifdef SOUND
    brailleSpeak.handleChar(getRandomLetter());
#endif
    delay(200);
    ledHelper.setEye(0, 0, 0);
  }

  // Cycle through modes if press is greater than 1.5 seconds
  if (touch_end - touch_start > 1500) {
    mode = mode + 1;
    if (mode >= MODE_SIZE) mode = MODE_LISTEN;
    show_mode();
//    Serial.println(mode);
  }
//  Serial.println(touch_end - touch_start);
  touch_start = NULL;

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

void show_mode()
{
  ledHelper.set(1, (mode == MODE_SECURITY) * 50, (mode == MODE_KNOCKBACK) * 50, (mode == MODE_LISTEN) * 50);
}



void loop()
{
  if (touch_start == NULL && digitalRead(BUTTON_PIN) == HIGH) touch_start = millis();
//  Serial.println(digitalRead(BUTTON_PIN));
  if (ignore_motion == 0) {
    if (digitalRead(MOTION_PIN) == HIGH) {
      last_movement = millis();
    }
  }
  else ignore_motion--;

  double sound = 0.0;
  ledHelper.breathe();
  switch (mode) {
    case MODE_KNOCKBACK:
      knockback.play();
      break;
    case MODE_SECURITY:
      //      Serial.println(last_movement);
//            Serial.print(last_movement);
//            Serial.print(",");
//            Serial.println(ignore_motion);
      if (last_movement == 0 || last_movement <= millis() - 3000) {
        ledHelper.setEye(0, 0, 0);
        if (last_movement != 0 && ignore_motion == 0) {
          ignore_motion = 500;
          last_movement = 0;
        }
      }
      else ledHelper.setEye(255, 255, 255);
      break;
    default:
      if (ENABLE_SLEEP && last_movement <= millis_minus_timeout()) {
        // I'm asleep becauase i haven't felt you move for SLEEP_TIMEOUT milliseconds
        ledHelper.setEye(0, 0, 0);
      }
      else {
        // I'm awake and listening to noises
        sound = listener.sample_sound();
        ledHelper.set(2, 0, 0, sound);
      }
      break;
  }
  ledHelper.set(0, 0, 0, ledHelper.breath_sin()); // @todo integrate into breathe() method and set to constant rate, not steps
  FastLED.show();
}

int millis_minus_timeout() {
  if (millis() < SLEEP_TIMEOUT) return 0;
  return millis() - SLEEP_TIMEOUT;
}
