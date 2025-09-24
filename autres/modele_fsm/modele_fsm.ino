enum AppState {STOP, STATE_A};

AppState appState = STOP;

unsigned long currentTime = 0;

// Les pragma region permettent de créer des zones de code fermable
#pragma region Modèles

void xState(unsigned long cT) {
  static unsigned long lastTime = 0;
  const int rate = 500;

  static bool firstTime = true;

  if (firstTime) {
    // Code d'initialisation de l'état
    // Reset tes trucs
    // Exemples : Angle de référence, initialiser le lastTime;

    firstTime = false;
    return;
  }

  if (cT - lastTime < rate) return;

  lastTime = cT;

  // Code de job à faire

  // Il s'agit de la transition qui permet de sortir de l'état
  // Qu'est-ce qui fait que l'on sort de l'état?
  bool transition = false;

  // Il est possible d'avoir plusieurs transitions

  if (transition) {
    // Code pour terminer la tâche

    firstTime = true;
  }
}

// Modèle de tâche qui ne retourne aucune valeur.
void xTask(unsigned long ct) {
  static unsigned long lastTime = 0;
  unsigned long rate = 500;
  
  if (ct - lastTime < rate) {
    return;
  }
  
  lastTime = ct;
  
  // Faire le code de la tâche ici
  
}

// Modèle de tâche avec retour de valeur.
// Exemple d'utilisation :
//   - Mesure de distance, température, etc.
int xTaskWithReturn(unsigned long ct) {
  static unsigned long lastTime = 0;
  unsigned long rate = 500;
  static int lastResult = 0;
  int result = 0;
  
  if (ct - lastTime < rate) {
    return result;
  }
  
  lastTime = ct;
  
  // Faire le code de la tâche ici
  result = 1;
  
  lastResult = result;
  
  return result;  
}

#pragma endregion

void stateManager(unsigned long ct) {
  // Adapter selon votre situation!
  switch (appState) {
    case STOP:
      
      break;
  }
}

#pragma region setup-loop
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  currentTime = millis();
  
  stateManager(currentTime);

}
#pragma endregion