#include <MeAuriga.h>

#define BATTMAX 613 // 7.2v/12v * 1023

int battLevel = 0; // Niveau de la batterie

MeLineFollower lineFollower(PORT_9);
MeEncoderOnBoard encoderRight(SLOT1);
MeEncoderOnBoard encoderLeft(SLOT2);

int moveSpeed = 75;
int lineFollowFlag = 0;
int error = 0;

unsigned long currentTime = 0;

unsigned long serialPrevious = 0;
int serialInterval = 500;
String msg = "";

void setup() {
  Serial.begin(115200);
  
  encoderSetup();
}

void loop() {
  currentTime = millis();
  
  byte lines = lineFollower.readSensors();
  
  switch (lines) {
    case S1_IN_S2_IN:
      error = 0;
      Forward();
      msg = "GAUCHE et DROITE voit la ligne";
      break;
    case S1_IN_S2_OUT:
      error--;
      speedAdjust(moveSpeed, error);
      msg = "GAUCHE voit la ligne et DROITE ne voit pas la ligne";
      break;
    case S1_OUT_S2_IN:
      error++;
      speedAdjust(moveSpeed, error);
      msg = "GAUCHE ne voit pas la ligne et DROITE voit la ligne";
      break;
    case S1_OUT_S2_OUT:
      if (error < 0) {
        SpinRight();
      } else if (error > 0) {
        SpinLeft();
      } else {
        Backward();
      }
      msg = "Les deux capteurs ne voit pas la ligne";
      break;
  }
  
  serialPrintTask();
  
  
}

void speedAdjust(int speed, int error) {
  encoderLeft.setMotorPwm(speed + error);  
  encoderRight.setMotorPwm(-speed + error);   
}

void Forward() {
  encoderLeft.setMotorPwm(moveSpeed);  
  encoderRight.setMotorPwm(-moveSpeed);  
}

void TurnLeft() {
  encoderLeft.setMotorPwm(moveSpeed / 2);  
  encoderRight.setMotorPwm(-moveSpeed);
}

void TurnRight() {
  encoderLeft.setMotorPwm(moveSpeed);  
  encoderRight.setMotorPwm(-moveSpeed / 2);  
}

void Backward() {
  encoderLeft.setMotorPwm(-moveSpeed / 2);  
  encoderRight.setMotorPwm(moveSpeed);  
}

void SpinRight() {
  encoderLeft.setMotorPwm(moveSpeed);  
  encoderRight.setMotorPwm(0);  
}

void SpinLeft() {
  encoderLeft.setMotorPwm(-0);  
  encoderRight.setMotorPwm(-moveSpeed);  
}

// Fonction d'interruption pour le moteur droit
void encoderRight_interrupt(void)
{
  if(digitalRead(encoderRight.getPortB()) == 0)
  {
    encoderRight.pulsePosMinus();
  }
  else
  {
    encoderRight.pulsePosPlus();;
  }
}

// Fonction d'interruption pour le moteur gauche
void encoderLeft_interrupt(void)
{
  if(digitalRead(encoderLeft.getPortB()) == 0)
  {
    encoderLeft.pulsePosMinus();
  }
  else
  {
    encoderLeft.pulsePosPlus();
  }
}

// Configuration des encodeurs
void encoderSetup() {
  attachInterrupt(encoderRight.getIntNum(), encoderRight_interrupt, RISING);
  attachInterrupt(encoderLeft.getIntNum(), encoderLeft_interrupt, RISING);
  
  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  
  encoderRight.setPosPid(1.8,0,1.2);
  encoderLeft.setPosPid(1.8,0,1.2);
  encoderRight.setSpeedPid(0.18,0,0);
  encoderLeft.setSpeedPid(0.18,0,0);
  
  // // Pour le capteur de ligne, nous n'avons pas besoin
  // // de gérer le PId des moteurs
  // encoderRight.setMotionMode(DIRECT_MODE);
  // encoderLeft.setMotionMode(DIRECT_MODE);
}

// Retourne le pourcentage de la batterie
int getBattLevel() {
  int value = analogRead(A4);
  return (int)((value * 100.0) / BATTMAX);
}


void serialPrintTask() {
  if (currentTime - serialPrevious < serialInterval) return;

  serialPrevious = currentTime;

  if (msg != "") {
    Serial.print("Batt : ");
    Serial.print(getBattLevel());
    Serial.print("\t");
    Serial.print(msg);
    Serial.println("");
    msg = "";
  }

}