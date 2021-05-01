

//--------------------------------------------------------------------------------------------------------------------------------------
// Librerias
//--------------------------------------------------------------------------------------------------------------------------------------

#include "pitches.h"
#define NOTE_C4_1 260

//--------------------------------------------------------------------------------------------------------------------------------------
// Variables  y notas musicales
//--------------------------------------------------------------------------------------------------------------------------------------

int buzzerPin = 40;
int ledblue= 19;
int buttonState = 0;

int melody[] = {                                               // cancion Uno
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
};

int noteDurations[] = {                                 // duracion cancion Uno
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


int melody2[] = {                                               // Cancion 2
  NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5, 
  NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5, 
  NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_FS5, 
  NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5
};

int durations[] = {                                             // duracion Cancion 2
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8, 
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8
};

int songLength = sizeof(melody)/sizeof(melody[0]);              // duracion y tamaño
int songLength2 = sizeof(melody2)/sizeof(melody2[0]);           // duracion y tamaño


//--------------------------------------------------------------------------------------------------------------------------------------
// Setup: Pines de salida y entrada para buzzer y recibir instrucciones de Tiva 1
//--------------------------------------------------------------------------------------------------------------------------------------

void setup() 
{
pinMode(ledblue,INPUT_PULLUP);
pinMode(buzzerPin,OUTPUT);
}


//--------------------------------------------------------------------------------------------------------------------------------------
// Loop: De acuerdo a duracion, se tocan las notas musicales
//--------------------------------------------------------------------------------------------------------------------------------------

void loop() 
{
  buttonState = digitalRead(ledblue);
  if (buttonState == HIGH) 
  {
    for (int thisNote = 0; thisNote < 130; thisNote++) 
    {
      int noteDuration = 1000/noteDurations[thisNote];
      tone(buzzerPin, melody[thisNote],noteDuration);
      int pauseBetweenNotes = noteDuration *1.30-50;             // delay por cada pulso
      delay(pauseBetweenNotes);
      delay(50);
      noTone(buzzerPin);                
    }
  }
  else if (buttonState == LOW)
  {
    for (int thisNote2 = 0; thisNote2 < songLength2; thisNote2++)
    {
      int duration2 = 1000/ durations[thisNote2];
      tone(buzzerPin, melody2[thisNote2], duration2);
      int pause = duration2 * 1.3;                               // delay por cada pulso
      delay(pause);
      noTone(8);
    }
  }  
}
