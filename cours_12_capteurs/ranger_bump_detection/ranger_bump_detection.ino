#include <MeAuriga.h>
#include <Wire.h>

unsigned long currentTime = 0;

MeGyro gyro(0, 0x69); // Constructeur

unsigned long serialPrevious = 0;
int serialInterval = 250;

unsigned long bumpPrevious = 0;
int bumpInterval = 10;
float bumpDetectLimit = 1;
char bumpDetected = 0;

float dx = 0.0;
float axp = 0;
float ax = 0;


void gyroTask() {
  gyro.update();

  ax = gyro.getAngleX();

  if (currentTime - bumpPrevious >= bumpInterval){
    bumpPrevious = currentTime;

    dx = abs((ax - axp) * 10);
    axp = ax;

    if (dx >= bumpDetectLimit) {
      bumpDetected = 1;
    }
  }

}

void serialOutputTask() {
  if (bumpDetected) {
    bumpDetected = 0;
    Serial.print("Collision detected!! --");
    Serial.print("dx:");
    Serial.println(dx);
  }


  if (currentTime - serialPrevious < serialInterval ) return;

  serialPrevious = currentTime;

  // Tache d'impression ici
}

void setup()
{
  Serial.begin(115200);
  gyro.begin();
}

void loop()
{
  currentTime = millis();

  gyroTask();
  serialOutputTask();
}