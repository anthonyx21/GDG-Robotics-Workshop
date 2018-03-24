#include <Servo.h>

#define outputA 6
#define outputB 7
#define buttonPin 5

Servo myservo;  // create servo object to control a 

int pos = 0;
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
bool isDoorOpen = false;

int counter = 0;
int aState;
int aLastState;
void setup() {
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);
  //   pinMode (buttonPin,INPUT);
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
}

void closeDoor(){
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
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
    Serial.println(F("Released"));
    if(!isDoorOpen){
      openDoor();
      isDoorOpen = true;
    }
    else{
      closeDoor();
      isDoorOpen = false;
    }
  }
}
