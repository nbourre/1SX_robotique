// À TESTER, JE N'AVAIS PAS D'ÉCRAN AU MOMENT D'ÉCRIRE LE CODE
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Largeur de l'écran OLED, en pixels
#define SCREEN_HEIGHT 64 // Hauteur de l'écran OLED, en pixels
#define OLED_RESET     -1 // Numéro de la broche de réinitialisation (ou -1 si partageant la broche de réinitialisation de l'Arduino)
#define SCREEN_ADDRESS 0x3C /// Lire la fiche technique ou exécuter i2c_scanner

Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool hasScreen = false;

unsigned long currentTime = 0;

String msg = "";

void setup() {
  Serial.begin(115200);
  hasScreen = ssd1306Init();

}

void loop() {
  currentTime = millis();
  // put your main code here, to run repeatedly:
  testScreenTask(currentTime);
}

#pragma region INITIALIZERS

bool ssd1306Init(){
  return screen.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

#pragma endregion

#pragma region SCREEN

void displayText(const char * txt, short y = 0, short x = 0, bool reset = 0) {
  if (!hasScreen) return;
  
  if (reset) {
    screen.clearDisplay();
  }
  
  screen.setCursor(x, y);
  screen.print(txt);
  screen.display();
}

#pragma endregion

#pragma region TASKS

void testScreenTask(unsigned long cT) {
  static char msg[20];
  
  static unsigned long lastTime = 0;
  const int rate = 500;
  
  if (cT - lastTime < rate) return;
  lastTime = cT;
  
  // Conversion de la valeur numérique en texte
  dtostrf(lastTime, 6, 2, msg);
  
  displayText (msg);
}

void serialPrintTask(unsigned long cT) {
  static unsigned long lastTime = 0;
  const int rate = 500;
  
  if (cT - lastTime < rate) return;
  lastTime = cT;

  if (msg != "") {
    Serial.println(msg);
    msg = "";
  }
}
#pragma endregion // CONSTANTS
