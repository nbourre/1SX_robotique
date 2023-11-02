#include <MeAuriga.h>

#define BATTMAX 613 // 7.2v/12v * 1023
#define DIST_WHEEL 151
#define DIA_WHEEL 64.5
#define PULSE 9
#define RATIO 39.267
#define FULL_TURN_CIRC 474.4
#define FULL_SPIN_CIRC 237.2
#define CIRC_WHEEL 202.6

int battLevel = 0; // Niveau de la batterie

unsigned long currentTime = 0;

MeEncoderOnBoard encoderRight(SLOT1);
MeEncoderOnBoard encoderLeft(SLOT2);

int moveSpeed = 100;

unsigned long movePrevious = 0;
int moveDelay = 2000;

// Variable que j'inverse pour indiquer la direction du mouvement
int moveDir = 1;

unsigned long serialPrevious = 0;
int serialInterval = 500;
String msg = "";

void setup() {
  Serial.begin(115200);
  
  encoderSetup();
}

void loop() {
  currentTime = millis();
  
  if (currentTime - movePrevious >= moveDelay) {
    movePrevious = currentTime;
    
    moveForward (200 * moveDir);
    moveDir *= -1;
  }
  
  msg = encoderLeft.distanceToGo(); // Pour afficher le nombre de degrés restant
  
  encoderLoop();
  serialPrintTask();
}

// Doit toujours être appelé pour se mettre à jour
void encoderLoop() {
  encoderLeft.loop();
  encoderRight.loop();
}

// Permet d'indiquer au robot d'avancer à une distance précise
bool moveForward(int dist) {
  float distance = dist / CIRC_WHEEL * 360;
  
  encoderLeft.move(distance);
  encoderRight.move(-distance);

  // Juste pour afficher la distance
  msg = String(distance);
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
  
  encoderRight.setPulse(PULSE);
  encoderLeft.setPulse(PULSE);
  
  encoderRight.setRatio(RATIO);
  encoderLeft.setRatio(RATIO);
  
  encoderRight.setPosPid(1.8,0,1.2);
  encoderLeft.setPosPid(1.8,0,1.2);
  
  encoderRight.setSpeedPid(0.18,0,0);
  encoderLeft.setSpeedPid(0.18,0,0);
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