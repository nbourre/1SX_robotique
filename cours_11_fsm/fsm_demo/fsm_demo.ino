#include <MeAuriga.h>

// la liste des états possible de notre système
// ainsi qu'une variable etatCourant prenant une de ces valeurs
enum {REPOS, ETAT_V, ETAT_VJ, ETAT_VJO, ETAT_VJOR} etatCourant;

#define LEDNUM  12

// on-board LED ring, at PORT0 (onboard)
MeRGBLed led( 0, LEDNUM );

unsigned long currentTime = 0;
 

void serialEvent() {
  if (!Serial.available()) return;

  char c = Serial.read();
  Serial.print("J'ai reçu : ");
  Serial.println(c);

  switch (c) {
    case '1':
      simpleClick();
      break;
  }
}

// ------------------------------------------------------
// Cette fonction installe l'état initial
// ------------------------------------------------------
void mettreAuRepos()
{
  showColor(0, 0, 0);
  etatCourant = REPOS;
}

// Attention, un index de 0 allume toutes les DELs
void showSegment(short startIdx, short nbLeds, uint8_t red, uint8_t green, uint8_t blue) {
  short endIdx = startIdx + nbLeds;
  for (short i = startIdx; i < endIdx; i++) {
    led.setColorAt(i, red, green, blue);
  }
  led.show();
}

void showColor(uint8_t red, uint8_t green, uint8_t blue) {
  led.setColor(red, green, blue);
  led.show();
}

void showGreen() {
  showSegment(0, 3, 0, 127, 0);
}

void showYellow() {
  showSegment(3, 3, 127, 127, 0);
}

void showOrange() {
  showSegment(6, 3, 127, 50, 0);
}

void showRed() {
  showSegment(9, 3, 127, 0, 0);
}

// On simule le click en envoyant la valeur 1 sur le port série  
void simpleClick() {
  switch (etatCourant) {
    case REPOS: // on était au repos et on a un appui, on allume la verte
      showGreen(); // DEL verte alimentée
      etatCourant = ETAT_V; // on note le nouvel état de notre système
      break;

    case ETAT_V: // on était led verte allumée et on a un appui, on allume la jaune
      showYellow(); // DEL jaune alimentée
      etatCourant = ETAT_VJ;// on note le nouvel état de notre système
      break;

    case ETAT_VJ: // vert et jaune allumées, on a un appui, on allume la orange
      showOrange(); // DEL orange alimentée
      etatCourant = ETAT_VJO;// on note le nouvel état de notre système
      break;

    case ETAT_VJO:// vert, orange et jaune allumées, on a un appui, on allume la rouge
      showRed(); // DEL rouge alimentée
      etatCourant = ETAT_VJOR;// on note le nouvel état de notre système
      break;

    case ETAT_VJOR: // tout était allumé, on a un appui, on retourne au repos
      mettreAuRepos(); // on retourne à l'état initial
      break;
  }
}

void initRing() {
  led.setpin( 44 );
  mettreAuRepos();
}

void setup() {
  Serial.begin(115200);
  initRing();
}

void loop() {
  currentTime = millis();  

  // ici on peut faire autre chose du moment que ça ne prend pas trop longtemps
  printTask(currentTime);
}

void printTask(unsigned long cT) {
  static unsigned long lastTime = 0;
  const int rate = 500;

  if (cT - lastTime < rate) return;
  lastTime = cT;

  Serial.println(etatCourant);

}