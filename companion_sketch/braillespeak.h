#ifndef BrailleSpeak_h
#define BrailleSpeak_h

#include "Arduino.h"

#define TONE_DURATION 200

class BrailleSpeak
{
    
  private:
    
    void outputTone(int t);
    int _pin;
    
  public:
    BrailleSpeak(int pin);
    void speak(char * words);
    void handleChar(char character);
};
#endif
