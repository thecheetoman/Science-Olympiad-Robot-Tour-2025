

//#include <MeOrion.h>
#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

int turnfactor = 850;
// The two motor objects
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

void setup()
{
  
}
void forward(float seconds){
  motor1.run(-205);
  motor2.run(200);
  delay(seconds);
  motor1.stop();
  motor2.stop();
  delay(100);
}
void turnLeft(){
  motor1.run(100);
  motor2.run(100);
  delay(turnfactor);
  motor1.stop();
  motor2.stop();
  delay(200);
}
void turnRight(){
  motor1.run(-100);
  motor2.run(-100);
  delay(turnfactor);
  motor1.stop();
  motor2.stop();
  delay(200);
}
void Main(){
  forward(20000);
}
void loop(){
  pinMode(A7, INPUT);
  while(1) {
      if((0 ^ (analogRead(A7) > 10 ? 0 : 1))){
        delay(1000);
        Main();
      }
  }
}
