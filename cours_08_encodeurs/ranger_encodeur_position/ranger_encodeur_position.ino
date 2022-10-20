#include <MeAuriga.h>

#define ENC_PULSE 9
#define ENC_RATIO 40
#define DIA 65.0
#define CIRC 20.2

unsigned long cT = 0;

unsigned long serialPrevious = 0;
int serialDelay = 250;

unsigned long movePrevious = 0;
int moveDelay = 5000;
int moveCounter = 0;

MeEncoderOnBoard Encoder_1(SLOT1);

void isr_process_encoder1(void)
{
  if(digitalRead(Encoder_1.getPortB()) == 0)
  {
    Encoder_1.pulsePosMinus();
  }
  else
  {
    Encoder_1.pulsePosPlus();;
  }
}


void setup()
{
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  Serial.begin(115200);
  
  // DÉBUT : Ne pas modifier ce code!
  // Configuration de la fréquence du PWM
  // Copier-coller ce code si on désire
  // travailler avec les encodeurs
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  // FIN : Ne pas modifier ce code!

  Encoder_1.setPulse(ENC_PULSE);
  Encoder_1.setRatio(ENC_RATIO);
  Encoder_1.setPosPid(1.8,0,1.2);
  Encoder_1.setSpeedPid(0.18,0,0);

  //gotoDistance(100.0);

}

void serialTask() {
  if (cT - serialPrevious < serialDelay) {
    return;
  }
  
  serialPrevious = cT;
  
  // Afficher la position du "curseur"
  Serial.print("Position 1:");
  Serial.print(Encoder_1.getCurPos());
  Serial.print(",Pulse:");
  Serial.print(Encoder_1.getPulsePos());
  Serial.print(",isTarReached:");
  Serial.print(Encoder_1.isTarPosReached());
  Serial.print(",distanceToGo:");
  Serial.println(Encoder_1.distanceToGo());
}

void moveTask() {
  if (cT - movePrevious < moveDelay) {
    return;
  }  

  movePrevious = cT;
  Encoder_1.setPulsePos(0);

  Encoder_1.moveTo(ENC_PULSE * ENC_RATIO);
  moveCounter++;
}

void gotoDistance(float distance) {
  float nbTours = distance / CIRC;
  float nbPulses = nbTours * ENC_PULSE * ENC_RATIO;

  Encoder_1.moveTo(nbPulses);

  
}

void loop()
{
  cT = millis();

  gotoDistance(100.0);

  Encoder_1.loop();
  serialTask();
}

