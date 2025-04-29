/* Code for Arduino Mega
    - Listen for Button Switch
    - Send Disable signals to Nano_x and Nano_y
    - Stepper Move nn steps
    - Close Servo
    - Stepper Move -nn steps (D36, D37)
    - Move x-left, y-down until switch is triggered (D12, D2)
    - Open Servo
    - Wait 5000ms
    - Loop
*/

#define ENABLE_X 46
#define ENABLE_Y 45
#define ENABLE_Z 47

#define ZSTEPDIR 37
#define ZSTEP 30

#define HIGHSPEEDPULSE 600
#define LOWSPEEDPULSE 650


// 32 33 2 a6(d60) a7(d61) rx
#define BUTTON 33

#define XLSWITCH 61
#define YDSWITCH 60
#define XLNANO 34 //fixed
#define YDNANO 35 //fixed

#define SERVO 32

#include <Servo.h>

Servo myservo;

int stepdelay = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(XLSWITCH, INPUT_PULLUP);
  pinMode(YDSWITCH, INPUT_PULLUP);

  pinMode(ZSTEPDIR, OUTPUT);
  pinMode(ZSTEP, OUTPUT);
  pinMode(XLNANO, OUTPUT);
  pinMode(YDNANO, OUTPUT);

  pinMode(SERVO, OUTPUT);

  pinMode(ZSTEPDIR, OUTPUT);
  pinMode(ZSTEP, OUTPUT);

  pinMode(ENABLE_X, OUTPUT);
  pinMode(ENABLE_Y, OUTPUT);
  pinMode(ENABLE_Z, OUTPUT);

  myservo.attach(SERVO);

  Serial.begin(9600);

  stepdelay = LOWSPEEDPULSE;
}

void loop() {
  // put your main code here, to run repeatedly:

    digitalWrite(ENABLE_X, LOW);
    delay(10);
    digitalWrite(ENABLE_Y, LOW);
    delay(10);
    digitalWrite(ENABLE_Z, LOW);
    delay(10);
    digitalWrite(XLNANO, HIGH);
    delay(10);
    digitalWrite(YDNANO, HIGH);
    delay(10);

    while(digitalRead(BUTTON) == LOW){

      digitalWrite(ENABLE_X, HIGH);
      delay(10);
      digitalWrite(ENABLE_Y, HIGH);
      delay(10);


      digitalWrite(ZSTEPDIR, HIGH);
      delay(100);
      for(int i=0; i<1750; i++){
        digitalWrite(ZSTEP, HIGH);
        delayMicroseconds(stepdelay);
        digitalWrite(ZSTEP, LOW);
        delayMicroseconds(stepdelay);
        myservo.write(85); //opening
      }


      myservo.write(90);  //pause
      delay(200);
      myservo.write(100); //closing
      delay(2000);
      myservo.write(95); // close as move up

      delay(200);

      digitalWrite(ZSTEPDIR, LOW); //Pull claw up
      delay(100);
      for(int i=0; i<1750; i++){
        digitalWrite(ZSTEP, HIGH);
        delayMicroseconds(stepdelay);
        digitalWrite(ZSTEP, LOW);
        delayMicroseconds(stepdelay);
      }

      digitalWrite(ENABLE_X, LOW);
      delay(10);
      digitalWrite(ENABLE_Y, LOW);
      delay(10);

      digitalWrite(XLNANO, LOW);
      delay(100);
      digitalWrite(YDNANO, LOW);
      delay(100);

      delay(5000); //Moving home

      myservo.write(85);
      delay(2000);
      myservo.write(100);
      delay(2000);
      myservo.write(89);
      delay(100);

      digitalWrite(XLNANO, HIGH);
      delay(10);
      digitalWrite(YDNANO, HIGH);
      delay(10);


    }



}
