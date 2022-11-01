// WiP : Not completed
#include <MeAuriga.h>

unsigned long cT = 0;

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
  if (cT - serialPrevious < serialDelay) {
    return;
  }
  
  serialPrevious = cT;
  
  Serial.print("Vitesse 1:");
  Serial.println(Encoder_1.getCurrentSpeed());
}

void loop()
{
  cT = millis();
  
  if(Serial.available())
  {
    char a = Serial.read();
    switch(a)
    {
      case '0':
      Encoder_1.setTarPWM(0);
      break;
      case '1':
      Encoder_1.setTarPWM(100);
      break;
      case '2':
      Encoder_1.setTarPWM(200);
      break;
      case '3':
      Encoder_1.setTarPWM(255);
      break;
      case '4':
      Encoder_1.setTarPWM(-100);
      break;
      case '5':
      Encoder_1.setTarPWM(-200);
      break;
      case '6':
      Encoder_1.setTarPWM(-255);
      break;
      default:
      break;
    }
  }
  Encoder_1.loop();
  serialTask();

}

