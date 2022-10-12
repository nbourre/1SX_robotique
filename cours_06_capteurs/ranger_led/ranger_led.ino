#include <MeAuriga.h>

#define LEDNUM  12
#define LEDPIN  44
#define RINGALLLEDS 0

enum Couleur {ROUGE, VERT, BLEU, MAX_COULEUR};

MeRGBLed led( PORT0, LEDNUM );

unsigned long currentTime = 0;

unsigned long ledPrevious = 0;
int ledDelay = 3000;

long color = 0xf03c15;
Couleur currentColor = ROUGE;

void setup() {
  led.setpin(LEDPIN);
  led.setColor(100, 100, 0 );
  led.show();
}

void loop() {  
  currentTime = millis();
  
  ledTask();    
}

void ledTask () {
  if (currentTime - ledPrevious < ledDelay) return;
  
  ledPrevious = currentTime;
 
  for (int i = 0 ; i <= LEDNUM; i++) {
    led.setColor(RINGALLLEDS, color);
  }
  led.show();    
  
  currentColor = (currentColor + 1) % MAX_COULEUR;
  
  switch(currentColor) {
    case ROUGE : 
      color = 0xf03c15;
      break;
    case VERT:
      color = 0xc5f015;
      break;
    case BLEU :
      color = 0x1589F0;
      break;
    default :
      color = 0xDEAD01;
  }
}
