/**
 * @file         mBot Ranger Autonomous Mode.ino
 * @author       IntoRobotics.com
 * @version      V1.1
 * @date         2016/05/13
 * @description  this file is sample code for the mBot Ranger kit
 */
 
#include "MeAuriga.h"
#include <SoftwareSerial.h>

MeUltrasonicSensor ultraSensor(PORT_10); // Ultrasonic module

int distanceLimit=50;    // Maximum distance we want to ping for (in centimeters).
int maxPwm=255;
int halfPwm=200;
int turnPwm=150;
int d=500;
int frontDistance;


//Motor Left
const int pwmMotor1 = 11;
const int inMotor1_1 = 49;
const int inMotor1_2 = 48;

//Motor Right
const int pwmMotor2 = 10;
const int inMotor2_1 = 47;
const int inMotor2_2 = 46;

void setup()
{
  Serial.begin(9600);
 
  pinMode(pwmMotor1,OUTPUT);    //We have to set PWM pin as output
  pinMode(inMotor1_1,OUTPUT);  //Logic pins are also set as output
  pinMode(inMotor1_2,OUTPUT);
  pinMode(pwmMotor2,OUTPUT);    //We have to set PWM pin as output
  pinMode(inMotor2_1,OUTPUT);  //Logic pins are also set as output
  pinMode(inMotor2_2,OUTPUT);
 
}

void loop()
{
   GetFrontDistance();  //Get the ultrasonic distance in cm
 
   if(frontDistance>=distanceLimit){
      FullSpeedMode();  //The robot is in full speed mode
    }

   else {  
      ReduceSpeed();   //Reduce the speed of the robot
      Stop();          //Stop the robot
      do {  
          TurnRight();
          delay(200);  // Wait for sensor to stabilize
          GetFrontDistance();  //Get the ultrasonic distance in cm  
          } while(frontDistance<distanceLimit);
      }
}

 int GetFrontDistance(){
   frontDistance=ultraSensor.distanceCm();
   Serial.print("Front distance (in cm) is: "); //For debugging
   Serial.println(frontDistance);
   return frontDistance;
   }

 void FullSpeedMode(){
    digitalWrite(inMotor1_1, HIGH);
    digitalWrite(inMotor1_2, LOW);
    analogWrite(pwmMotor1,maxPwm);
    digitalWrite(inMotor2_1, LOW);
    digitalWrite(inMotor2_2, HIGH);
    analogWrite(pwmMotor2,maxPwm);
    Serial.println("Full speed mode"); //For debugging
   }

  void ReduceSpeed(){
    digitalWrite(inMotor1_1, HIGH);
    digitalWrite(inMotor1_2, LOW);
    analogWrite(pwmMotor1,halfPwm);//Set speed via PWM
    digitalWrite(inMotor2_1, LOW);
    digitalWrite(inMotor2_2, HIGH);
    analogWrite(pwmMotor2,halfPwm);//Set speed via PWM
    Serial.println("Reduce the speed"); //For debugging
    delay(d);
    }

  void Stop(){
    analogWrite(pwmMotor1, 0);
    analogWrite(pwmMotor2, 0);
    Serial.println("Stop");
    delay(d);
   }
 
   void TurnRight(){
    digitalWrite(inMotor1_1, HIGH);
    digitalWrite(inMotor1_2, LOW);
    analogWrite(pwmMotor1,turnPwm);//Set speed via PWM
    digitalWrite(inMotor2_1, HIGH);
    digitalWrite(inMotor2_2, LOW);
    analogWrite(pwmMotor2,turnPwm);//Set speed via PWM
    Serial.println("Turn the robot right"); //For debugging
    }