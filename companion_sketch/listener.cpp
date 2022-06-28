#include "Arduino.h"
#include "listener.h"

Listener::Listener(int pin)
{
  pinMode(pin, INPUT);
  _pin = pin;
}


double Listener::sample_sound() {
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  
  // collect data for 50 mS
  while (millis() - startMillis < SOUND_SAMPLE_WINDOW)
  {
    unsigned int sample = analogRead(_pin);
    
    if (sample < 1024)  // toss out spurious readings
    {
       if (sample > signalMax)
       {
          signalMax = sample;  // save just the max levels
       }
       else if (sample < signalMin) // @todo this will never be hit because of the parent 'if'. Review this.
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
