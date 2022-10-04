#include "AurigaMotor.h"

AurigaMotor motorRight;
AurigaMotor motorLeft;

void setup() {
  // put your setup code here, to run once:
  motorRight.setpin(48, 49, 11);
  motorLeft.setpin(47, 46, 10);
}

void loop() {
  // put your main code here, to run repeatedly:
  motorRight.run(255);
  motorLeft.run(255);
  delay(500);
  motorRight.run(-255);
  motorLeft.run(-255);
  delay(500);
  motorRight.stop();
  motorLeft.stop();
  delay(3000);
}
