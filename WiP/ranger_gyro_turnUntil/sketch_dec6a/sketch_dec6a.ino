#include <MeAuriga.h>

unsigned long currentTime = 0;
unsigned int dt = 0;
unsigned long previousTime = 0;

MeGyro gyro(0, 0x69);

void gyroTask() {
  gyro.update();

  ax = gyro.getAngleX();

}

void setup() {
  Serial.begin(9600);

  gyro.begin();

}

void loop() {
  currentTime = millis();
  dt = currentTime - previousTime;
  previousTime = currentTime;



}
