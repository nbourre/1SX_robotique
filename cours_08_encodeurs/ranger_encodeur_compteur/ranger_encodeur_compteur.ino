#include <MeAuriga.h>

unsigned long currentTime = 0;

unsigned long serialPrevious = 0;
unsigned long serialDelay = 250;

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
}

void serialTask() {
  if (currentTime - serialPrevious < serialDelay) {
    return;
  }
  
  serialPrevious = currentTime;
  
  // Afficher la position du "curseur"
  Serial.print("Position 1:");
  Serial.print(Encoder_1.getCurPos());
  Serial.print(",Pulse:");
  Serial.println(Encoder_1.getPulsePos());
}

void loop()
{
  currentTime = millis();
  
  Encoder_1.loop();
  serialTask();
}

