/* Code for Arduino Nano
    - Listen for x-limitswitches, x-joystick, Enable, Move-left pin
    - Send signal accordingly
*/

// X is the one on the side of red
// 9, 10, 11, 12

#define SPEEDPULSE 300

#define JOYXUPIN 10
#define JOYXDOWNIN 9

#define XLEFTSWITCH 12
#define XRIGHTSWITCH 11

#define MOVELEFTPIN 4

#define STEPXPIN 7
#define DIRXPIN 8


void setup() {
  // put your setup code here, to run once:
  pinMode(JOYXUPIN, INPUT_PULLUP);
  pinMode(JOYXDOWNIN, INPUT_PULLUP);

  pinMode(XLEFTSWITCH, INPUT_PULLUP);
  pinMode(XRIGHTSWITCH, INPUT_PULLUP);

  pinMode(STEPXPIN, OUTPUT);
  pinMode(DIRXPIN, OUTPUT);

  pinMode(MOVELEFTPIN, INPUT_PULLUP); // LOW

  pinMode(LED_BUILTIN, OUTPUT);


}


// Blocking Mode -> If too laggy, change to non-blocking mode..?
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, LOW);
  if(digitalRead(MOVELEFTPIN) == LOW){

    while(digitalRead(XLEFTSWITCH) != LOW){ // Move until Switch is Triggered
      //stepper.move(-MICROSTEPS * 50);
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(DIRXPIN, LOW);
      delay(2);
      digitalWrite(STEPXPIN, HIGH);
      delayMicroseconds(SPEEDPULSE);
      digitalWrite(STEPXPIN, LOW);
      delayMicroseconds(SPEEDPULSE);
    }
    delay(1000);
    //stepper.disable(); // allow to move by hand
    //delay(3000); // wait
  }else{
    if(digitalRead(JOYXUPIN) == LOW){
      if(digitalRead(XRIGHTSWITCH) == LOW) return;
      digitalWrite(DIRXPIN, HIGH);
      delay(2);
    }else if(digitalRead(JOYXDOWNIN) == LOW){
      if(digitalRead(XLEFTSWITCH) == LOW) return;
      digitalWrite(DIRXPIN, LOW);
      delay(2);
    }else{
      return;
    }
    digitalWrite(STEPXPIN, HIGH);
    delayMicroseconds(SPEEDPULSE);
    digitalWrite(STEPXPIN, LOW);
    delayMicroseconds(SPEEDPULSE);
  }
}
