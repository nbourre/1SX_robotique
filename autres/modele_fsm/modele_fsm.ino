
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

  // Remplacer par la formule pour effectuer la transition
  bool transition = false;

  // Il est possible d'avoir plusieurs transitions

  if (transition) {
    // Code pour terminer la tâche

    firstTime = true;
  }
}

#pragma endregion

#pragma region setup-loop
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

}
#pragma endregion