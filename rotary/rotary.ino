#include <Servo.h>

#define outputA 6
#define outputB 7
#define buttonPin 5

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
void setup() {
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);
  myButton.begin();
  myservo.attach(9);

  Serial.begin (9600);
  // Reads the initial state of the outputA
  aLastState = digitalRead(outputA);
}

void openDoor(){
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  isDoorOpen = true;
}

void closeDoor(){
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  isDoorOpen = false;
}

void readRotaryState(){
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

    if(isDoorOpen){
      closeDoor();
      currentIndex = 0;
      currentMode = challenge;
    }
    else{
      
      if(currentMode == record){
        sequence[currentIndex] = counter;
        currentIndex++;
        for(int i = 0; i < 3; i++){
          Serial.println(sequence[i]);
        }
        Serial.println("DONE");
  
        if(currentIndex == 3){
          currentMode = challenge;
          Serial.println("SWITCHING TO CHALLENGE MODE");
          currentIndex = 0;
        }
      }
      else{
//       if( sequence[currentIndex] != counter){
        Serial.println(sequence[currentIndex]);
        Serial.println(counter);
       if( abs(sequence[currentIndex] - counter) > 5 ){
        currentIndex = 0;
        Serial.println("INCORRECT");
       }
       else{
        currentIndex++;
        Serial.println("OK");
        if(currentIndex == 3){
          Serial.println("ALL RIGHT");
          currentIndex = 0;
          openDoor();
        }
     }
  }
}
    
  }
}
