
#include "Arduino.h"
#include "ledhelper.h"
#include <FastLED.h> // Manage Libraries -> Install FastLED
  
LEDHelper::LEDHelper()
{
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(_leds, _count);
  _breath_pause = millis();
  _breath = 0;
  _look_pos = 6;
  _look_next = millis();
  _blink_state = false;
  _blink_next = millis();
}

void LEDHelper::set(int identifier, int r, int g, int b) {
  _leds[identifier] = CRGB(r, g, b);
}

void LEDHelper::setEye(int r, int g, int b) {
  for (int i = 0; i < 7; i++) set(i, r, g, b);
}

void LEDHelper::setAll(int r, int g, int b) {
  for (int i = 0; i < 9; i++) set(i, r, g, b);
}

int LEDHelper::breath_sin() {
  return (sin(_breath * 0.0174533) + 1) * 80 - 80;
}

int LEDHelper::breathe() {
  int next;
  if (_breath_pause > millis() - 2000) {
    return;
  }
  if (_breathe_in == true) {
    next = _breath = _breath + BREATH_STEP;
    if (_breath > 80) {
      next = 80;
      _breathe_in = false;
    }
  }
  else {
    next = _breath = _breath - BREATH_STEP;
    if (_breath < 0) {
      next = 0;
      _breath_pause = millis();
      _breathe_in = true;
    }
  }
  _breath = next;
  return next;
}

void LEDHelper::do_blink() {
  if (_blink_next > millis()) return;
  if (!_blink_state) _blink_next = millis() + BLINK_LENGTH;
  else _blink_next = millis() + random(3000, 10000);
  _blink_state = !_blink_state;
  
  set(_look_pos, 0, 0, _blink_state ? 0 : EYE_BRIGHTNESS);
}

void LEDHelper::look() {
  do_blink();
  if (_look_next > millis()) return;
  setEye(0, 0, 0);
  switch (_look_pos) {
    case 6:
      _look_pos = 3;
      _look_next = millis() + LOOK_PAUSE;
      break;
    case 3:
      _look_pos = 0;
      _look_next = millis() + LOOK_PAUSE;
      break;
    case 0:
      _look_pos = 6;
      _look_next = millis() + random(10000, 50000);
      break;
  }
  set(_look_pos, 0, 0, _blink_state ? 0 : EYE_BRIGHTNESS);
}
