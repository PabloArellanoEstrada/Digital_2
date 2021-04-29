/* 
 Educational BoosterPack MK II - Birthday Tune
http://boosterpackdepot.info/wiki/index.php?title=Educational_BoosterPack_MK_II
 
 Play birthday tune through the buzzer, demonstrating
 buzzer tune() API and pitch/tone (hence music) generation
 
 Dec 2012 - Created for Educational BoosterPack
            buzzer Pin = 19
 Dec 2013 - Modified for Educational BoosterPack MK II
            buzzer Pin = 40
 by Dung Dang
 
 */
#include "pitches.h"
#define NOTE_C4_1 260


int buzzerPin = 40;
int ledblue= 19;
int buttonState = 0;

// notes in the melody:
int melody[] = {
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_A3,
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_E4, 0,
  NOTE_D5, NOTE_A4,
  NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_D4,
  
  NOTE_C4, NOTE_D4, NOTE_A3,
  NOTE_D5, NOTE_A4,
  NOTE_F4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, 
  NOTE_F4, NOTE_G4, NOTE_E4, NOTE_C4,
  NOTE_D4, NOTE_A3, NOTE_D4,
  NOTE_D5, NOTE_A4,
  
  NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_D4,
  NOTE_C4, NOTE_D4, NOTE_A3,
  NOTE_D5, NOTE_A4, 
  NOTE_F4, NOTE_G4, NOTE_A4, NOTE_D4,
  NOTE_E4, NOTE_G4, NOTE_E4, NOTE_C4,
  
  NOTE_D4,
  NOTE_G2, NOTE_G2, NOTE_AS2, NOTE_D3, 
  NOTE_F3, NOTE_E3, NOTE_D3, NOTE_A2,
  NOTE_G2, NOTE_G2, NOTE_AS2, NOTE_D3,
  NOTE_F3, NOTE_G3, NOTE_A3, 
  NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4,
  
  NOTE_A4, NOTE_D4, NOTE_A4,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4,
  NOTE_F4, NOTE_G4, NOTE_A4, NOTE_F4,
  NOTE_G2, NOTE_G2, NOTE_AS2, NOTE_D3, 
  NOTE_F3, NOTE_E3, NOTE_D3, NOTE_A2,
  NOTE_G2, NOTE_G2, NOTE_AS2, NOTE_D3,
  
  NOTE_F3, NOTE_G3, NOTE_A3,
  NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_AS4, NOTE_C5, 
  NOTE_D5, NOTE_A4, NOTE_F4, NOTE_D4, 
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_C4,
  NOTE_D4, NOTE_D4, NOTE_C4,
  
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_E4, 0, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4,
  
  NOTE_AS3, NOTE_AS3, NOTE_AS3, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4,NOTE_C4, NOTE_AS3,
  NOTE_A3, NOTE_A3, NOTE_A3, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_C4, 
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_AS4, NOTE_GS4,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_GS4, NOTE_G4, NOTE_A4, NOTE_G4,
  
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4, 
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4,
  
  NOTE_AS3, NOTE_AS3, NOTE_AS3, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_G4, 
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_E4, NOTE_G4,
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_D5, NOTE_CS5, 0
}

// note durations: 4 = quarter note, 8 = eighth note, etc.:

;int noteDurations[] = {
 4, 8, 8, 8, 8, 4,
  8, 8, 8, 8, 4, 4,
  2, 2,
  4, 4, 2,
  4, 4, 4, 4,
  
  4, 4, 2,
  2, 2,
  4, 4, 4, 8, 8,
  4, 4, 4, 4,
  4, 4, 2,
  2, 2,
  
  4, 4, 2,
  4, 4, 4, 4,
  4, 4, 2,
  2, 2,
  4, 4, 4, 4,
  4, 4, 4, 4,
  
  1,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 2,
  3, 8, 4, 8, 8,
  
  4, 4, 2, 
  8, 16, 16, 8, 8, 4, 8, 8,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  
  4, 4, 2,
  3, 8, 4, 8, 8,
  4, 4, 4, 4, 
  8, 16, 16, 8, 8, 8, 8, 8, 8,
  2, 4, 4,
  
  8, 16, 16, 8, 8, 8, 16, 16, 8, 8,
  8, 16, 16, 8, 8, 4, 8, 16, 16,
  8, 16, 16, 8, 8, 8, 16, 16, 8, 8,
  8, 16, 16, 8, 8, 3, 16, 16,
  
  8, 16, 16, 8, 8, 8, 16, 16, 8, 8, 
  8, 16, 16, 8, 8, 4, 4,
  8, 16, 16, 8, 8, 8, 16, 16, 8, 8,
  8, 16, 16, 8, 8, 3, 16, 16,
  
  8, 16, 16, 8, 8, 8, 16, 16, 8, 8,
  8, 16, 16, 8, 8, 3, 16, 16,
  8, 16, 16, 8, 8, 8, 16, 16, 8, 8,
  8, 16, 16, 8, 8, 3, 16, 16,
  
  8, 16, 16, 8, 8, 8, 16, 16, 8, 8, 
  8, 16, 16, 8, 8, 4, 4,
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 4, 4
  };


  // The melody array 
int melody2[] = {
  NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5, 
  NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5, 
  NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_FS5, 
  NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5
};

// The note duration, 8 = 8th note, 4 = quarter note, etc.
int durations[] = {
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8, 
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8
};

int songLength = sizeof(melody)/sizeof(melody[0]);
int songLength2 = sizeof(melody2)/sizeof(melody2[0]);

void setup() 
{
pinMode(ledblue,INPUT_PULLUP);
pinMode(buzzerPin,OUTPUT);

}
void loop() 
{
  buttonState = digitalRead(ledblue);
  if (buttonState == HIGH) 
    {
  for (int thisNote = 0; thisNote < 130; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration *1.30-50; //delay between pulse
    delay(pauseBetweenNotes);
    delay(50);
    
    noTone(buzzerPin);                // stop the tone playing
  }
  }

  else if (buttonState == LOW)
    {
    for (int thisNote2 = 0; thisNote2 < songLength2; thisNote2++){
    // determine the duration of the notes that the computer understands
    // divide 1000 by the value, so the first note lasts for 1000/8 milliseconds
    int duration2 = 1000/ durations[thisNote2];
 
    tone(buzzerPin, melody2[thisNote2], duration2);
    // pause between notes
    int pause = duration2 * 1.3;
    delay(pause);
    // stop the tone
    noTone(8);
  }
    }
    
    
  
}
