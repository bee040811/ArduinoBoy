//
/*
   Melody

   Plays a melody 

circuit:
 * 8-ohm speaker on digital pin 8

 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe 

 This example code is in the public domain.

http://arduino.cc/en/Tutorial/Tone

/*
  NOTE_C5 Do
  NOTE_D5 RE
  NOTE_E5 Mi
  NOTE_F5 Fa
  NOTE_G5 So
  NOTE_A5 La
  NOTE_B5 Si
  NOTE_C6 high Do
*/
#include "pitches.h"

#define Do NOTE_C5
#define Re NOTE_D5 
#define Mi NOTE_E5
#define Fa NOTE_F5 
#define So NOTE_G5 
#define La NOTE_A5
#define Si NOTE_B5
#define HDo NOTE_C6

// notes in the melody:
int melody[] = {
NOTE_E4, NOTE_E4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_G3,   
NOTE_C4, NOTE_G3, NOTE_E3, NOTE_A3, NOTE_B3, NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_D4, NOTE_B3,  
NOTE_C4, NOTE_G3, NOTE_E3, NOTE_A3, NOTE_B3, NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_D4, NOTE_B3,  
NOTE_G4, NOTE_FS4, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_GS3, NOTE_A3, NOTE_C4, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_C5, NOTE_C5, NOTE_C5,  
NOTE_G4, NOTE_FS4, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_GS3, NOTE_A3, NOTE_C4, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_C4,  
NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_G3, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4,  
NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_G3
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
8,4,4,8,4,2,2, 
3,3,3,4,4,8,4,
8,8,8,4,8,4,3,
8,8,3,3,3,3,4,
4,8,4,8,8,8,4,
8,4,3,8,8,2,8,
8,8,4,4,8,8,4,
8,8,3,8,8,8,4,
4,4,8,2,8,8,8,
4,4,8,8,4,8,8,
3,3,3,1,8,4,4,
8,4,8,4,8,2,8,
4,4,8,4,1,8,4,
4,8,4,8,4,8,2
};

void setup(void) 
{
    Serial.begin(9600); 
}

void loop(void) 
{
    // no need to repeat the melody.
    // iterate over the notes of the melody:
    int size = sizeof(noteDurations) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
        Serial.print(thisNote);
        // to calculate the note duration, take one second 
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 2*(1000/noteDurations[thisNote]);
        tone(8, melody[thisNote],noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        //noTone(8);
    }
}

