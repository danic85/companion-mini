#ifndef LEDHelper_h
#define LEDHelper_h

#include "Arduino.h"

#include <FastLED.h> // Manage Libraries -> Install FastLED
#define LED_COUNT 9
#define LED_PIN   10

#define BREATH_STEP   3
#define LOOK_PAUSE    1000
#define BLINK_LENGTH  300
#define EYE_BRIGHTNESS 30

class LEDHelper
{
    
  private:
    CRGB _leds[LED_COUNT];
    int _count = LED_COUNT;
    
    int _breath;
    bool _breathe_in = true;
    unsigned long _breath_pause;
    
    unsigned long _blink_next;
    boolean _blink_state;
    
    unsigned long _look_next;
    unsigned long _look_pos;
    
  public:
    LEDHelper();
    void set(int identifier, int r, int g, int b);
    void setEye(int r, int g, int b);
    void setAll(int r, int g, int b);
    int breath_sin();
    int breathe();
    void look();
    void do_blink();
};
#endif
