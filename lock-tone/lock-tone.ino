#include"pitches.h"

// notes in the song 'Mukkathe Penne'
int melody[] = {
NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_AS4, NOTE_A4,                   
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,0,                                 

NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4,      //29               //8
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,0,                                       

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5,
NOTE_C4, NOTE_D4, NOTE_DS4, NOTE_FS4, NOTE_D5, NOTE_C5,
NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_AS4,             //58

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_G4,
NOTE_A4, 0, NOTE_AS4, NOTE_A4, 0, NOTE_G4,
NOTE_G4, NOTE_A4, NOTE_G4, NOTE_FS4, 0,

NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, 0,
NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, END

};

// note durations: 8 = quarter note, 4 = 8th note, etc.
int noteDurations[] = {       //duration of the notes
8,4,8,4,
4,4,4,12,
4,4,4,4,4,4,
4,16,4,

8,4,8,4,
4,2,1,1,2,1,1,12,
4,4,4,4,4,4,
4,16,4,

4,4,4,4,4,4,
4,4,4,12,
4,4,4,4,4,4,
4,4,4,12,

4,4,4,4,4,4,
2,1,1,2,1,1,4,8,4,
2,6,4,2,6,4,
2,1,1,16,4,

4,8,4,4,4,
4,16,4,
4,8,4,4,4,
4,20,
};

int speed=90;  //higher value, slower notes
int pin = 3;
 
void setup() {

  Serial.begin(9600);
  
  for (int thisNote = 0; melody[thisNote]!=-1; thisNote++) {
  
    int noteDuration = speed*noteDurations[thisNote];
    tone(pin, melody[thisNote]); //pin, frequency and duration
    Serial.println(melody[thisNote]);
    
    delay(noteDuration);
    
    noTone(3);
  }
}



//
//int lockMelody[] = {
//    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_A4, END
//  };
//
//  int lockDuration[] = {3, 3, 3, 5};
//  int lockSpeed = 150;
//  int isLocked = true;
//  
//int unlockMelody[] = {
//    NOTE_A4, NOTE_A4, NOTE_A4, NOTE_D4, END
//  };
//
//  int unlockDuration[] = {3, 3, 3, 5};
// 
//void setup() {
//
//  Serial.begin(9600);
//  
//  
//}
//
//
//void lockDoor() {
//  
//  for(int note = 0; lockMelody[note] != -1; note ++) {
//    Serial.println(note);
//    tone(pin, lockMelody[note], lockDuration[note]*80);
//    delay(lockSpeed * lockDuration[note]); // pause for amount of time
//    noTone(pin);
//  }
//}
//
//
//void unlockDoor() {
//  for(int note = 0; unlockMelody[note] != -1; note ++) {
//    Serial.println(note);
//    tone(pin, unlockMelody[note], lockDuration[note]*80);
//    delay(lockSpeed * lockDuration[note]); // pause for amount of time
//    noTone(pin);
//  }
//}
//
//
//
//void loop() {
//// no need to repeat the melody.
//if(isLocked) {
//    unlockDoor();
//    isLocked = false;
//  } else {
//    lockDoor();
//    isLocked = true;
//  }
//
//
//  delay(10000);
//}


void loop() {}
