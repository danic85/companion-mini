#ifndef Listener_h
#define Listener_h

#include "Arduino.h"

#define SOUND_SAMPLE_WINDOW 50
#define MODIFIER 150

class Listener
{
    
  private:
    int _pin;
    
  public:
    Listener(int pin);
    double sample_sound();
};
#endif
