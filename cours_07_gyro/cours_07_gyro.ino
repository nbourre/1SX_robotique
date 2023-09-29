#include <MeAuriga.h>

MeGyro gyro(0, 0x69);

unsigned long currentTime = 0;


String msg = "";

void setup() {
  Serial.begin(9600);
  gyro.begin();

}

void loop() {
  currentTime = millis();
  // put your main code here, to run repeatedly:
  gyroTask(currentTime);
  serialPrintTask(currentTime);
}

void gyroTask(unsigned long ct) {
  
  gyro.update();
  
}

void serialPrintTask(unsigned long ct) {
  static unsigned long lastTime = 0;
  const int rate = 500;
  
  if (ct - lastTime < rate) return;
  
  lastTime = ct;
  
  msg = "x:";
  msg += gyro.getAngleX();
  msg += ",y:";
  msg += gyro.getAngleY();
  msg += ",z:";
  msg += gyro.getAngleZ();

  

  if (msg != "") {
    Serial.println(msg);
    msg = "";
  }

}