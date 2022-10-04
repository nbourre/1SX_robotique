#include "Arduino.h"
#include "AurigaMotor.h"

AurigaMotor::AurigaMotor() {

}


AurigaMotor::AurigaMotor(uint8_t port) {

}

AurigaMotor::AurigaMotor(uint8_t ena_pin, uint8_t enb_pin, uint8_t pwm_pin) {

}

void AurigaMotor::setpin(uint8_t ena_pin, uint8_t enb_pin, uint8_t pwm_pin)
{
  ena = ena_pin;
  enb = enb_pin;
  pwm = pwm_pin;

  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(pwm, OUTPUT);
}


void AurigaMotor::run(int16_t speed)
{
  speed	= speed > 255 ? 255 : speed;
  speed	= speed < -255 ? -255 : speed;
  
  if(last_speed != speed)
  {
    last_speed = speed;
  }
  else
  {
    return;
  }

  if(speed >= 0)
  {
    digitalWrite(ena, HIGH);
    digitalWrite(enb, LOW);
    analogWrite(pwm, speed);
  }
  else
  {
    digitalWrite(ena, LOW);
    digitalWrite(enb, HIGH);    
    analogWrite(pwm, -speed);
  } 
    
}

void AurigaMotor::stop() {
  analogWrite(pwm, 0);
}