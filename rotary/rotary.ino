#include <Servo.h>
#include"pitches.h"


#define outputA 6
#define outputB 7
#define buttonPin 5

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

enum mode {
  record,
  challenge
};

mode currentMode = record;

class Button {
  private:
    bool _state;
    uint8_t _pin;

  public:
    Button(uint8_t pin) : _pin(pin) {}

    void begin() {
      pinMode(_pin, INPUT_PULLUP);
      _state = digitalRead(_pin);
    }

    bool isReleased() {
      bool v = digitalRead(_pin);
      if (v != _state) {
        _state = v;
        if (_state) {
          return true;
        }
      }
      return false;
    }
};

Button myButton(buttonPin);
int sequence[3];
bool isDoorOpen = false;

int counter = 0;
int aState;
int aLastState;

int currentIndex = 0;


Servo myservo;  // create servo object to control a

int pos = 0;

int lockMelody[] = {
    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_A4, END
  };

  int lockDuration[] = {3, 3, 3, 5};
  int lockSpeed = 150;
  int isLocked = true;
  
int unlockMelody[] = {
    NOTE_A4, NOTE_A4, NOTE_A4, NOTE_D4, END
  };

  int unlockDuration[] = {3, 3, 3, 5};
  
 void lockDoor() {
  
  for(int note = 0; lockMelody[note] != -1; note ++) {
    Serial.println(note);
    tone(pin, lockMelody[note], lockDuration[note]*80);
    delay(lockSpeed * lockDuration[note]); // pause for amount of time
    noTone(pin);
  }
}

void playSong(){
  for (int thisNote = 0; melody[thisNote]!=-1; thisNote++) {
  
    int noteDuration = speed*noteDurations[thisNote];
    tone(pin, melody[thisNote]); //pin, frequency and duration
    Serial.println(melody[thisNote]);
    
    delay(noteDuration);
    
    noTone(3);
  }
}

void unlockDoor() {
  for(int note = 0; unlockMelody[note] != -1; note ++) {
    Serial.println(note);
    tone(pin, unlockMelody[note], lockDuration[note]*80);
    delay(lockSpeed * lockDuration[note]); // pause for amount of time
    noTone(pin);
  }
}

void setup() {
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);
  myButton.begin();
  myservo.attach(9);

  Serial.begin (9600);
  // Reads the initial state of the outputA
  aLastState = digitalRead(outputA);
}

void openDoor() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  isDoorOpen = true;

  playSong();
}

void closeDoor() {
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  isDoorOpen = false;
  lockDoor();
}

void readRotaryState() {
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState) {
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState) {
      counter ++;
    } else {
      counter --;
    }
    Serial.print("Position: ");
    Serial.println(counter);
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
}

void loop() {

  readRotaryState();

  if (myButton.isReleased()) {

    if (isDoorOpen) {
      closeDoor();
      currentIndex = 0;
      currentMode = challenge;
    }
    else {

      if (currentMode == record) {
        sequence[currentIndex] = counter;
        currentIndex++;
        for (int i = 0; i < 3; i++) {
          Serial.println(sequence[i]);
        }
        Serial.println("DONE");

        if (currentIndex == 3) {
          currentMode = challenge;
          Serial.println("SWITCHING TO CHALLENGE MODE");
          currentIndex = 0;
          
          unlockDoor();
        }
      }
      else {
        //       if( sequence[currentIndex] != counter){
        Serial.println(sequence[currentIndex]);
        Serial.println(counter);
        if ( abs(sequence[currentIndex] - counter) > 5 ) {
          currentIndex = 0;
          Serial.println("INCORRECT");

          unlockDoor();
        }
        else {
          currentIndex++;
          Serial.println("OK");
          if (currentIndex == 3) {
            Serial.println("ALL RIGHT");
            currentIndex = 0;
            openDoor();
          }
        }
      }
    }

  }
}
