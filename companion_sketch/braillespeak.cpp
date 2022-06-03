#include "Arduino.h"
#include "pitches.h"
#include "braillespeak.h"

// Each character in alphabetical order converted to decimal equivelant of
// binary braille pairs.
const int brailleMap[][2] = {
  {4, 0},
  {6, 0},
  {4, 4},
  {4, 6},
  {4, 2},
  {6, 4},
  {6, 6},
  {6, 2},
  {2, 4},
  {2, 6},
  {5, 0},
  {7, 0},
  {5, 4},
  {5, 6},
  {5, 2},
  {7, 4},
  {7, 6},
  {7, 2},
  {3, 4},
  {3, 6},
  {5, 1},
  {7, 1},
  {2, 7},
  {5, 5},
  {5, 7},
  {5, 3}
};

// Each decimal from binary mapped to note.
const int toneMap[] {
  NOTE_G3,
  NOTE_GS3,
  NOTE_AS3,
  NOTE_B3,
  NOTE_CS4,
  NOTE_D4,
  NOTE_E4,
  NOTE_F4
};
  
BrailleSpeak::BrailleSpeak(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void BrailleSpeak::speak(char * words) {
  for (int i = 0; i < strlen(words); i ++)
  {
    handleChar(words[i]);
  }
}

void BrailleSpeak::handleChar(char character)
{
  // Handle spaces.
  if (character == ' ')
  {
    delay(TONE_DURATION);
    return;
  }
  
  // Get tones from braille character map
  int brailleChars[2] = {0,0};
  int pos = ((int)character - (int)'A');
  
  // Handle lowercase values
  if (pos > 25)
  {
    pos -= 32;
  }

  // If it isn't a valid uppercase value, don't do anything
  if (pos < 0 || pos > 25)
  {
    return;
  }
  
  brailleChars[0] = brailleMap[pos][0];
  brailleChars[1] = brailleMap[pos][1];
  int tone  = toneMap[brailleChars[0]];
  BrailleSpeak::outputTone(tone);
  tone  = toneMap[brailleChars[1]];
  BrailleSpeak::outputTone(tone);
   
}

void BrailleSpeak::outputTone(int t)
{
  // Play tone for duration
  tone(_pin, t, TONE_DURATION);
  delay(TONE_DURATION);
  // Stop tone
  noTone(_pin);
}
