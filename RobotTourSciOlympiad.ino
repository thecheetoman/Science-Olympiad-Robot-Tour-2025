//inport stinky linbraris +=D(ad) im going ca ryazy
#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
//used to adjust turn time, adjust based on floor grippyness
int turnfactor = 850;
int turnfactorW4 = 560;


const int TARGET_M1 = -255;   // keep your tuned values
const int TARGET_M2 =  244;
// The two motor objects
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
const unsigned long BASE_TRAVEL_MS = 1300; // 50 cm
const unsigned long RAMP_MS = 1200;          // pick 300-1500ms typical
const unsigned long STEP_MS = 8;           // smoothness (smaller = smoother)
//nothing =D
void setup()
{
  //Nothing here yet, this ones once when the robot starts up and the robot needs to work off the button, not the power button
  
}
//move forward based on the time in miliseconds inputted
void forward(float seconds){
  motor1.run(-215);
  motor2.run(200);
  delay(seconds);
  motor1.stop();
  motor2.stop();
  delay(100);
}

//maybe turn left?
void turnLeft(){
  motor1.run(100);
  motor2.run(100);
  delay(turnfactorW4);
  motor1.stop();
  motor2.stop();
  delay(300);
}
//maybe turn right?
void turnRight(){
  motor1.run(-100);
  motor2.run(-100);
  delay(turnfactorW4);
  motor1.stop();
  motor2.stop();
  delay(300);
}
void rampTo(int start1, int start2, int end1, int end2, unsigned long rampMs) {
  if (rampMs == 0) {
    runMotors(end1, end2);
    return;
  }


  unsigned long steps = rampMs / STEP_MS;
  if (steps < 1) steps = 1;


  for (unsigned long i = 0; i <= steps; i++) {
    float t = (float)i / (float)steps;  // 0.0 -> 1.0
    int pwm1 = start1 + (int)((end1 - start1) * t);
    int pwm2 = start2 + (int)((end2 - start2) * t);
    runMotors(pwm1, pwm2);
    delay(STEP_MS);
  }
}
void runMotors(int pwm1, int pwm2) {
  motor1.run(pwm1);
  motor2.run(pwm2);
}
void stopMotors() {
  motor1.stop();
  motor2.stop();
}


//move one foot, added the slow down for consistencey
void oneSquare(){
  unsigned long ramp_ms = RAMP_MS;
  if (ramp_ms > BASE_TRAVEL_MS) ramp_ms = BASE_TRAVEL_MS; // safety
  unsigned long cruise_ms = BASE_TRAVEL_MS - ramp_ms;
  // Ramp up: 0 -> target
  rampTo(0, 0, TARGET_M1, TARGET_M2, ramp_ms);
  // Cruise at target
  runMotors(TARGET_M1, TARGET_M2);
  delay(cruise_ms);
  // Ramp down: target -> 0
  rampTo(TARGET_M1, TARGET_M2, 0, 0, ramp_ms);
  // Stop
  stopMotors();
  delay(100);
}
//the function that SHOULD be modified
void Main(){
  oneSquare();
}
//Dont touch this makes it move when the button is pressed
void loop(){
  pinMode(A7, INPUT);
  while(1) {
      if((0 ^ (analogRead(A7) > 10 ? 0 : 1))){
        delay(1000);
        Main();
      }
  }
}
