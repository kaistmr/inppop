
/*
  ard_mega_fixed.ino
  One–file version that replaces the previous ard_mega.ino + module files.

  Key fixes requested by An Yeon‑su (2025‑05‑11):
  1.  Z‑axis enable pin moved to 47 (was 46) to avoid PWM collision.
  2.  Limit‑switch polling interval shortened to 1 ms (no external interrupts).
  3.  Sequential homing routine (X → Y → Z) to stop belt skip.
  4.  Minor refactor to flatten code and remove isEnabled flag confusion.

  NOTE
  ----
  •  Timer‑Servo collision remains unresolved by request.
  •  Tune MOTOR_TIMEOUT / SPEED_* constants to your mechanics if needed.
*/

#include <Servo.h>

// ─────────────────────────────────────────────────────────────
// Pin map
// ─────────────────────────────────────────────────────────────
constexpr uint8_t EN_X = 41;      // Timer 5 PWM (conflicts with Servo but kept)
constexpr uint8_t EN_Y = 46;      // Timer 5 PWM
constexpr uint8_t EN_Z = 47;      // ← Changed to avoid double‑use of 46

constexpr uint8_t DIR_X1 = 37;    // H‑bridge IN1
constexpr uint8_t DIR_X2 = 39;    // H‑bridge IN2
constexpr uint8_t DIR_Y1 = 42;
constexpr uint8_t DIR_Y2 = 44;
constexpr uint8_t DIR_Z1 = 43;
constexpr uint8_t DIR_Z2 = 45;

constexpr uint8_t LIM_X_MIN = 9;
constexpr uint8_t LIM_X_MAX = 11;
constexpr uint8_t LIM_Y_MIN = 15;
constexpr uint8_t LIM_Y_MAX = 17;
constexpr uint8_t LIM_Z_MIN = 19;     // optional (wired to INT2)

// Joystick (digital 4‑way)
constexpr uint8_t JX_P = 23;
constexpr uint8_t JX_M = 25;
constexpr uint8_t JY_P = 27;
constexpr uint8_t JY_M = 29;

// Gripper servo
constexpr uint8_t SERVO_PIN = 34;

// ─────────────────────────────────────────────────────────────
// Constants
// ─────────────────────────────────────────────────────────────
constexpr uint16_t SPEED_FAST = 200;   // full PWM
constexpr uint16_t SPEED_SLOW = 60;    // slow for homing
constexpr uint16_t BACK_OFF_MM = 3;    // dummy (timing based)
constexpr uint32_t POLL_DELAY_US = 1000; // 1 ms

// ─────────────────────────────────────────────────────────────
// Utility
// ─────────────────────────────────────────────────────────────
struct Axis {
  uint8_t en, in1, in2, limMin, limMax;
  inline void setupPins() const {
    pinMode(en, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(limMin, INPUT_PULLUP);
    pinMode(limMax, INPUT_PULLUP);
    analogWrite(en, 0);                                  // disabled
  }
  inline void setSpeed(int16_t pwm) const {
    pwm = constrain(pwm, -255, 255);
    if (pwm == 0) {
      analogWrite(en, 0);
      return;
    }
    digitalWrite(in1, pwm > 0 ? HIGH : LOW);
    digitalWrite(in2, pwm > 0 ? LOW  : HIGH);
    analogWrite(en, abs(pwm));
  }
  inline void hardStop() const { analogWrite(en, 0); }
  inline bool atMin() const { return !digitalRead(limMin); }
  inline bool atMax() const { return !digitalRead(limMax); }
};

// Axis instances
const Axis axisX{EN_X, DIR_X1, DIR_X2, LIM_X_MIN, LIM_X_MAX};
const Axis axisY{EN_Y, DIR_Y1, DIR_Y2, LIM_Y_MIN, LIM_Y_MAX};
const Axis axisZ{EN_Z, DIR_Z1, DIR_Z2, LIM_Z_MIN, LIM_Z_MIN}; // only MIN used

Servo gripper;

// ─────────────────────────────────────────────────────────────
// Homing helpers
// ─────────────────────────────────────────────────────────────
void backOff(const Axis& a, int dir, uint32_t ms) {
  a.setSpeed(dir * -SPEED_SLOW);  // reverse
  delay(ms);
  a.hardStop();
}

void seekLimit(const Axis& a, int dir, bool toMin) {
  a.setSpeed(dir * SPEED_FAST);
  while (true) {
    if ((toMin && a.atMin()) || (!toMin && a.atMax())) break;
    delayMicroseconds(POLL_DELAY_US);
  }
  a.hardStop();
  delay(10);
  backOff(a, dir, 200);           // mechanical release
  a.setSpeed(dir * SPEED_SLOW);   // re‑approach slowly
  while (true) {
    if ((toMin && a.atMin()) || (!toMin && a.atMax())) break;
    delayMicroseconds(POLL_DELAY_US);
  }
  a.hardStop();
}

// ─────────────────────────────────────────────────────────────
// Setup / Loop
// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  axisX.setupPins();
  axisY.setupPins();
  axisZ.setupPins();

  pinMode(JX_P, INPUT_PULLUP);
  pinMode(JX_M, INPUT_PULLUP);
  pinMode(JY_P, INPUT_PULLUP);
  pinMode(JY_M, INPUT_PULLUP);

  gripper.attach(SERVO_PIN);
  gripper.write(90);   // neutral
  
  // Sequential homing
  seekLimit(axisX, -1, true);     // X‑ to min
  seekLimit(axisY, +1, false);    // Y+ to max
  seekLimit(axisZ, -1, true);     // Z‑ to min
  Serial.println(F("Homing done."));
}

void loop() {
  // Joystick → direction
  int dx = (!digitalRead(JX_P) ? +1 : 0) + (!digitalRead(JX_M) ? -1 : 0);
  int dy = (!digitalRead(JY_P) ? +1 : 0) + (!digitalRead(JY_M) ? -1 : 0);

  // Safety: stop at limits
  if (axisX.atMin() && dx < 0) dx = 0;
  if (axisX.atMax() && dx > 0) dx = 0;
  if (axisY.atMin() && dy < 0) dy = 0;
  if (axisY.atMax() && dy > 0) dy = 0;

  axisX.setSpeed(dx * SPEED_FAST);
  axisY.setSpeed(dy * SPEED_FAST);

  // Z & gripper demo (replace with real logic)
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'u') axisZ.setSpeed(+SPEED_FAST);
    else if (c == 'd') axisZ.setSpeed(-SPEED_FAST);
    else if (c == 's') axisZ.hardStop();
    else if (c == 'o') gripper.write(0);
    else if (c == 'c') gripper.write(180);
  }

  delayMicroseconds(POLL_DELAY_US);   // 1 ms loop pace
}
