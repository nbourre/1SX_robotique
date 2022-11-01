#include <MeAuriga.h>

#define BATTMAX 613 // 7.2v/12v * 1023

int battLevel = 0; // Niveau de la batterie

MeLineFollower lineFollower(PORT_9);
MeEncoderOnBoard encoderRight(SLOT1);
MeEncoderOnBoard encoderLeft(SLOT2);

int moveSpeed = 120;
int lineFollowFlag = 0;

int error = 0;
int goal = 0; // L'objectif est de n'avoir aucune erreur

unsigned long currentTime = 0;
unsigned long previousTime = 0;
int dt = 0;

unsigned long serialPrevious = 0;
int serialInterval = 500;
String msg = "";

void setup() {
  Serial.begin(115200);
  
  encoderSetup();
}


int multFactor = 1;
int sub = 0;
float pidCorr = 0;
int speedLeft;
int speedRight;
bool leftIR = 0;
bool rightIR = 0;
int andResult = 0;
int nor = 0;

void loop() {
  currentTime = millis();
  dt = currentTime - previousTime;
  previousTime = currentTime;
  
  leftIR = lineFollower.readSensor1();
  rightIR = lineFollower.readSensor2();
  
  int sub = leftIR - rightIR;
  
  andResult = leftIR && rightIR;
    
  // switch (lines) {
  //   case S1_IN_S2_IN:
  //     error = 0;
  //     break;
  //   case S1_IN_S2_OUT:
  //     error = -1;
  //     break;
  //   case S1_OUT_S2_IN:
  //     error = 1;
  //     break;
  //   case S1_OUT_S2_OUT:
  //     error = 0;
  //     break;
  // }
  
  
  
  pidCorr = calculatePid(sub, 0, 3, 0, 0.7, dt);
  
  if (andResult) {
    multFactor = 25;
  } else {
    multFactor = 1;
  }
  
  nor = !(leftIR & righIR);
  
  pidCorr = multFactor * pidCorr;
  
  // if (lines == S1_OUT_S2_OUT) {
  //   pidCorr *= 25;
  // }
  
  speedLeft = moveSpeed + pidCorr;
  speedRight = -(moveSpeed - pidCorr);
  
  encoderLeft.setMotorPwm(speedLeft);  
  encoderRight.setMotorPwm(speedRight);  
  
  //serialPrintTask();
  Serial.print("sub:");
  Serial.print(sub);
  Serial.print("\tmultFactor:");
  Serial.print(multFactor);
  Serial.print("\tpidCorr:");
  Serial.print(pidCorr);
  Serial.print("\tLeft:");
  Serial.print(speedLeft);
  Serial.print("\tRight:");
  Serial.println(speedRight);
  
  delay(100);
  
}

float errSum = 0;
float errPrev = 0;
float errD = 0;

float calculatePid(float real, float target, float p, float i, float d, int delta) {
  float err = target - real;
  
  errSum += err;
  
  errD = (err - errPrev) / delta;
  
  errPrev = err;
  
  return (p * err) + (i * errSum) + (d * errD);
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

void Spin() {
  encoderLeft.setMotorPwm(moveSpeed);  
  encoderRight.setMotorPwm(moveSpeed);  
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
  
  msg = "Left : ";
  msg += leftIR;
  msg += ", Right : ";
  msg += rightIR;
  

  if (msg != "") {
    Serial.print("Batt : ");
    Serial.print(getBattLevel());
    Serial.print("\t");
    Serial.print(msg);
    Serial.println("");
    msg = "";
  }

}