#ifndef KnockBack_h
#define KnockBack_h

#include "Arduino.h"
#include "ledhelper.h"

#define BUFFER_SIZE     40                      // Size of knock array
#define REPLY_TIMEOUT   2000                    // Number of milliseconds to wait before replaying knock pattern
#define THRESHOLD       400                     // Threshold value to decide when the detected sound is a knock or not
class KnockBack
{

  private:
    byte _sensor;
    byte _buzzer;
    LEDHelper* _ledhelper;

    int _sensorReading = 0;                      // variable to store the value read from the sensor pin
    unsigned long _knocks[BUFFER_SIZE];          // Knock 'buffer' - stores timing of each knock ready for replaying.
    int _knockPos = 0;                           // Current position within knocks array
    unsigned long _lastKnock = false;            // Time of last knock
  
  public:
    KnockBack(int sensor, int buzzer, LEDHelper* ledhelper);
    play();
    handle(int reading);
    replay();
    flash(bool buzz);
};
#endif
