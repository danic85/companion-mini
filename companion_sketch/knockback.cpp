/* Echoing Knock Sensor
 
 This sketch reads a piezo element to detect a knocking sound. 
 It reads an analog pin and compares the result to a set threshold. 
 If the result is greater than the threshold, it stores the value of millis() to an array.
 Following a defined period without additional knocks, the knocks are replayed by the device.
 
 Created by Dan Nicholson <http://dannicholson.co.uk>
 Based on "Knock Sensor" created 25 Mar 2007 by David Cuartielles <http://www.0j0.org> modified 30 Aug 2011 by Tom Igoe
 
 This code is in the public domain.
 
*/

#include "Arduino.h"
#include "knockback.h"
#include <FastLED.h> // Manage Libraries -> Install FastLED
  
KnockBack::KnockBack(int sensor, int buzzer, LEDHelper* ledhelper)
{
  _sensor = sensor;
  _buzzer = buzzer;
  _ledhelper = ledhelper;
}

KnockBack::play()
{
  _sensorReading = analogRead(_sensor); // read the sensor
//  Serial.println(_sensorReading);
  if (_lastKnock && (_lastKnock + REPLY_TIMEOUT) < millis()) replay(); // Replay knocks if last knock was older than the defined wait time
  else if (_sensorReading >= THRESHOLD) handle(_sensorReading); // If knock detected, handle
}

KnockBack::handle(int reading)
{
  _lastKnock = millis();
  
  if (_knockPos < BUFFER_SIZE)
  {
    _knocks[_knockPos++] = _lastKnock; // Store timing and increment position variable
    flash(false);
  }
}

KnockBack::replay()
{
    int i;
    _lastKnock = false;
//    Serial.println("Replaying");
    for (i = 0; i < _knockPos; i = i + 1) 
    {
      int prev = i-1;
      if(prev < 0) prev = 0;
      unsigned long d = (_knocks[i] - _knocks[prev]);
      if (d > 50) d = d-50;
      delay(d);
//      Serial.println(i);
      flash(true);
    }
    _knockPos = 0; // reset pointer;
}

KnockBack::flash(bool buzz) 
{
  _ledhelper->setEye(0, 0, 255);
  FastLED.show();
  if (buzz && _buzzer != NULL) tone(_buzzer, 1000);
  delay(50);
  _ledhelper->setEye(0, 0, 0);
  FastLED.show();
  if (buzz && _buzzer != NULL) noTone(_buzzer);
}
