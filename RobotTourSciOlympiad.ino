//inport stinky linbraris +=D(ad) im going ca ryazy
#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
//used to adjust turn time, adjust based on floor grippyness
int turnfactor = 850;
// The two motor objects
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
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
  delay(turnfactor);
  motor1.stop();
  motor2.stop();
  delay(200);
}
//maybe turn right?
void turnRight(){
  motor1.run(-100);
  motor2.run(-100);
  delay(turnfactor);
  motor1.stop();
  motor2.stop();
  delay(200);
}
//move one foot, added the slow down for consistencey
void foot(){
  motor1.run(-200);
  motor2.run(200);
  delay(1097);
  motor1.run(-50);
  motor2.run(50);
  delay(300);
  motor1.stop();
  motor2.stop();
  delay(200);
}
//the function that SHOULD be modified
void Main(){
  foot();
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