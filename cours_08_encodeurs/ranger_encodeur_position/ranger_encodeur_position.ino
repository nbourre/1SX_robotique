#include <MeAuriga.h>

unsigned long cT = 0;

unsigned long serialPrevious = 0;
unsigned long serialDelay = 250;

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
  
  Encoder_1.setPulse(9);
  Encoder_1.setRatio(39.267);
  
  Encoder_1.setPosPid(1.8, 1, 1.2);
  Encoder_1.setSpeedPid(0.18, 0, 0); 
}

void setup()
{
  attachInterrupt(Encoder_1.getIntNum(), interruption_encodeur_1, RISING);
  Serial.begin(115200);
  
  configureEncoders();
}

void serialOutputTask() {
  if (cT - serialPrevious < serialDelay) {
    return;
  }
  
  serialPrevious = cT;
    
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

void loop()
{
  cT = millis();
  
  readSerialCommand();
  
  Encoder_1.loop();
  serialOutputTask();
}

