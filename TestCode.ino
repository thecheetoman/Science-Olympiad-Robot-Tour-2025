#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// tuned motor speeds for straight driving
const int TARGET_M1 = -255;
const int TARGET_M2 = 244;

const unsigned long TURN_TOTAL_MS = 670;   // total turn duration (adjust)
const unsigned long TURN_RAMP_MS  = 300;   // accel/decel time

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

// set some variables for the move one square function.
const unsigned long BASE_TRAVEL_MS = 1300;  // ~50 cm
const unsigned long RAMP_MS = 1200;         // accel/decel time
const unsigned long STEP_MS = 8;            // ramp smoothness

void setup() {
  // Empty: robot starts only when button is pressed
}

// Simple forward movement (no ramp)
void forward(float ms) {
  motor1.run(-215);
  motor2.run(200);
  delay(ms);
  motor1.stop();
  motor2.stop();
  delay(100);
}

// 90 degree left turn
void turnLeft() {
  unsigned long ramp_ms = TURN_RAMP_MS;
  if (ramp_ms > TURN_TOTAL_MS) ramp_ms = TURN_TOTAL_MS;

  unsigned long cruise_ms = TURN_TOTAL_MS - ramp_ms;
  unsigned long stepsUp = ramp_ms / STEP_MS;
  if (stepsUp < 1) stepsUp = 1;

  for (unsigned long i = 0; i <= stepsUp; i++) {
    float t = (float)i / (float)stepsUp;
    int pwm = (int)(100 * t);
    motor1.run(pwm);
    motor2.run(pwm);
    delay(STEP_MS);
  }
  motor1.run(100);
  motor2.run(100);
  delay(cruise_ms);
  unsigned long stepsDown = ramp_ms / STEP_MS;
  if (stepsDown < 1) stepsDown = 1;

  for (unsigned long i = 0; i <= stepsDown; i++) {
    float t = (float)i / (float)stepsDown;
    int pwm = 100 - (int)(100 * t);  
    motor1.run(pwm);
    motor2.run(pwm);
    delay(STEP_MS);
  }

  motor1.stop();
  motor2.stop();
  delay(150);
}

// 90 degree right turn
void turnRight() {
  unsigned long ramp_ms = TURN_RAMP_MS;
  if (ramp_ms > TURN_TOTAL_MS) ramp_ms = TURN_TOTAL_MS;
  unsigned long cruise_ms = TURN_TOTAL_MS - ramp_ms;
  unsigned long stepsUp = ramp_ms / STEP_MS;
  if (stepsUp < 1) stepsUp = 1;
  for (unsigned long i = 0; i <= stepsUp; i++) {
    float t = (float)i / (float)stepsUp;
    int pwm = (int)(100 * t);
    motor1.run(-pwm);
    motor2.run(-pwm);
    delay(STEP_MS);
  }
  motor1.run(-100);
  motor2.run(-100);
  delay(cruise_ms);
  unsigned long stepsDown = ramp_ms / STEP_MS;
  if (stepsDown < 1) stepsDown = 1;
  for (unsigned long i = 0; i <= stepsDown; i++) {
    float t = (float)i / (float)stepsDown;
    int pwm = 100 - (int)(100 * t);
    motor1.run(-pwm);
    motor2.run(-pwm);
    delay(STEP_MS);
  }
  motor1.stop();
  motor2.stop();
  delay(150);
}
// start motors at specific speeds
void runMotors(int pwm1, int pwm2) {
  motor1.run(pwm1);
  motor2.run(pwm2);
}

// stop both motors
void stopMotors() {
  motor1.stop();
  motor2.stop();
}


void ForwardSquare() {
  unsigned long ramp_ms = RAMP_MS;
  if (ramp_ms > BASE_TRAVEL_MS) ramp_ms = BASE_TRAVEL_MS;

  unsigned long cruise_ms = BASE_TRAVEL_MS - ramp_ms;

  // sped up or smtng
  unsigned long stepsUp = ramp_ms / STEP_MS;
  if (stepsUp < 1) stepsUp = 1;

  for (unsigned long i = 0; i <= stepsUp; i++) {
    float t = (float)i / (float)stepsUp;
    int pwm1 = (int)(TARGET_M1 * t);
    int pwm2 = (int)(TARGET_M2 * t);
    runMotors(pwm1, pwm2);
    delay(STEP_MS);
  }

  // vroom
  runMotors(TARGET_M1, TARGET_M2);
  delay(cruise_ms);

  // vroom down
  unsigned long stepsDown = ramp_ms / STEP_MS;
  if (stepsDown < 1) stepsDown = 1;

  for (unsigned long i = 0; i <= stepsDown; i++) {
    float t = (float)i / (float)stepsDown;
    int pwm1 = TARGET_M1 - (int)(TARGET_M1 * t);
    int pwm2 = TARGET_M2 - (int)(TARGET_M2 * t);
    runMotors(pwm1, pwm2);
    delay(STEP_MS);
  }

  stopMotors();
  delay(100);
}
// Main behavior (edit this)
void Main() {
  turnLeft();
  turnRight();
}

// Wait for button press, then run Main()
void loop() {
  pinMode(A7, INPUT);

  while (1) {
    if ((0 ^ (analogRead(A7) > 10 ? 0 : 1))) {
      delay(1000);
      Main();
    }
  }
}