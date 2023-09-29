#include <MeAuriga.h>

#define ENC_PULSE 9
#define ENC_RATIO 40
#define DIA 65.0
#define CIRC 20.2

unsigned long currentTime = 0;

volatile long position_pulsation = 0;

float goal = 0.0;

MeEncoderOnBoard Encoder_1(SLOT1);

void interruption_encodeur_1(void)
{
  if(digitalRead(Encoder_1.getPortB()) == 0)
  {
    Encoder_1.pulsePosMinus();
    position_pulsation--;
  }
  else
  {
    Encoder_1.pulsePosPlus();
    position_pulsation++;
  }
}

void configureEncoders() {
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

void setup()
{
  attachInterrupt(Encoder_1.getIntNum(), interruption_encodeur_1, RISING);
  Serial.begin(115200);
  
  configureEncoders();
}

void loop()
{
  currentTime = millis();
  
  readSerialCommand();
  
  // Important! Permet de mettre à jour l'encodeur
  Encoder_1.loop();

  serialOutputTask(currentTime);
}

void serialOutputTask(unsigned long cT) {
  unsigned long lastTime = 0;
  const int rate = 250;

  if (cT - lastTime < rate) return;
  
  lastTime = currentTime;
    
  // Afficher les informations sur
  // le Serial Plotter
  Serial.print("Goal:");
  Serial.print(goal);
  Serial.print(",CurrentPosition:");
  Serial.print(Encoder_1.getCurPos());
  Serial.print(",Distance:");
  Serial.println(Encoder_1.distanceToGo());
}

// Fonction pour lire les commandes reçues par série
void readSerialCommand() {
  if (!Serial.available()) return;
  
  // Permet de convertir une valeur reçue en float.
  goal = Serial.parseFloat();
  
  Serial.print("New goal : ");
  Serial.println(goal);
  
  Encoder_1.moveTo(goal);
}