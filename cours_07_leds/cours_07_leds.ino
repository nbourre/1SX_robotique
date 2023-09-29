#include <MeAuriga.h>

#define LEDNUM  12
#define LEDPIN  44
#define RINGALLLEDS 0

MeRGBLed led( PORT0, LEDNUM );

unsigned long currentTime = 0;


void setup() {
  led.setpin(LEDPIN); // OBLIGATOIRE Configuration de la broche
}

void loop() {
  currentTime = millis();  
  ledTask(currentTime);
}

void ledTask(unsigned long cT) {
  static short idx = 1; // 0 = anneau complet
  static unsigned long lastTime = 0;
  int rate = 100;
  
  if (cT - lastTime < rate) return;
  
  lastTime = cT;
  // led.setColor(100, 100, 0); // Configure la couleur jaune
  led.setColor (0, 0, 0); // reset tous les leds
  led.setColor(idx, 0, 0, 5);
  
  idx = idx >= LEDNUM ? 1 : idx + 1;
  
  led.show(); // Active l'anneau avec la couleur
  
}
