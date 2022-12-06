#include <MeAuriga.h>
#include <Wire.h>

unsigned long currentTime = 0;
unsigned int dt = 0;
unsigned long previousTime = 0;

int moveSpeed = 30;

int angleGoal = 0;
int angleCurrent = 0;
int deadBand = 1;
int factor = 10;
int diffAngle = 0;
bool goalNew = false;

unsigned long serialOutputPrevious = 0;
int serialOutputDelay = 250;

MeGyro gyro(0, 0x69);
MeEncoderOnBoard encoderRight(SLOT1);
MeEncoderOnBoard encoderLeft(SLOT2);

void gyroTask() {

  angleCurrent = gyro.getAngleZ();
  gyro.update();
}

void encoderTask() {
  encoderLeft.loop();
  encoderRight.loop();
}

bool turnUntil(float degrees) {
  bool result = false;

  diffAngle = angleCurrent - angleGoal;

  if (abs(diffAngle) < deadBand) {
    result = true;
  }

  if (!result) {
    if (diffAngle > 0) {
      encoderLeft.setMotorPwm(-moveSpeed * 2);  
      encoderRight.setMotorPwm(-moveSpeed * 2);
    } else {
      encoderLeft.setMotorPwm(moveSpeed * 2);  
      encoderRight.setMotorPwm(moveSpeed * 2);
    }
  } else {
    stopMotor();
  }

  return result;
}

void stopMotor(){
  encoderLeft.setMotorPwm(0);  
  encoderRight.setMotorPwm(0);  
  goalNew = false;
}


void isr_process_encoder1(void)
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

void isr_process_encoder2(void) {
  if(digitalRead(encoderLeft.getPortB()) == 0)
  {
    encoderLeft.pulsePosMinus();
  }
  else
  {
    encoderLeft.pulsePosPlus();
  }
}

void encoderConfig() {
  attachInterrupt(encoderRight.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(encoderLeft.getIntNum(), isr_process_encoder2, RISING);
  
  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  encoderRight.setPulse(9);
  encoderLeft.setPulse(9);
  encoderRight.setRatio(39.267);
  encoderLeft.setRatio(39.267);
  encoderRight.setPosPid(1.8,0,1.2);
  encoderLeft.setPosPid(1.8,0,1.2);
  encoderRight.setSpeedPid(0.18,0,0);
  encoderLeft.setSpeedPid(0.18,0,0);
}

// Fonction pour lire les commandes reçues par série
void serialInputTask() {
  if (!Serial.available()) return;
  
  // Permet de convertir une valeur reçue en float.
  angleGoal = Serial.parseInt();
  
  if (angleGoal == 999) {
    stopMotor();
    return;
  }

  angleGoal = angleCurrent + angleGoal;

  Serial.print("New goal : ");
  Serial.println(angleGoal);
  goalNew = true;
}

void serialOutputTask() {
  if (currentTime - serialOutputPrevious < serialOutputDelay) return;

  serialOutputPrevious = currentTime;

  Serial.print("currentAngle:");
  Serial.print(angleCurrent);
  Serial.print("diffAngle:");
  Serial.println(diffAngle);  
}

void setup() {
  Serial.begin(9600);
  encoderConfig();

  gyro.begin();

}

void loop() {
  currentTime = millis();
  dt = currentTime - previousTime;
  previousTime = currentTime;

  serialInputTask();

  if (goalNew) {
    goalNew = !turnUntil(angleGoal);
  }

  gyroTask();
  encoderTask();

  serialOutputTask();
}
