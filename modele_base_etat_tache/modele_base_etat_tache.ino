
unsigned long currentTime = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  currentTime = millis();

  // Supprimer cet appel, c'est juste pour tester
  tacheTestSimulateurs(currentTime);

  // Laisser là pour les simulateurs
  tacheMiseAJourSimulateurs();
}

void tacheTestSimulateurs(unsigned long ct) {
  static unsigned long previousTime = ct;
  static unsigned long rate = 1000;  // 1 seconde
  
  if (currentTime - previousTime < rate) return;
  
  previousTime = currentTime;
  
  Serial.print("--- Test de simulation --- ");
  Serial.println(ct / 1000.0);
  Serial.print("Temperature : "); Serial.println(simGetTemperature());
  Serial.print("Humidité : "); Serial.println(simGetHumidity());
  Serial.print("Distance : "); Serial.println(simGetDistance());
  Serial.print("Bouton : "); Serial.println(simDigitalRead(0));
  Serial.print("Luminosité : "); Serial.println(simGetLuminosity());
}

void modeleState(unsigned long ct) {
  static unsigned long previousTime = ct;
  static unsigned long rate = 1000;  // 1 seconde
  static bool firstTime = true;


  if (firstTime) {
    firstTime = false;
    // Code à exécuter une seule fois au début
    // Initialisation des variables
    Serial.println("Entrée État : Modèle");
  }

  // Si nécessaire
  if (ct - previousTime < rate) {
    return;
  }

  previousTime = ct;

  // Code à exécuter à chaque appel de la fonction
  // Mise à jour des variables

  bool transition = false;  // Mettre à vrai si on change d'état

  if (transition) {
    // Code à exécuter une seule fois à la fin
    // Nettoyage des variables
    Serial.println("Sortie État : Modèle");

    firstTime = true;
  }

  // Vous pouvez ajouter d'autres transitions ici
}

void modeleTask(unsigned long ct) {
  static unsigned long previousTime = 0;
  static unsigned long rate = 1000;  // 1 seconde

  if (ct - previousTime < rate) {
    return;
  }

  previousTime = ct;

  // Code à exécuter à chaque appel de la fonction
  // Mise à jour des variables
}

#pragma region Simulateurs

// Toujours appeler dans la boucle principale
// pour mettre à jour la simulation;
void tacheMiseAJourSimulateurs() {
  simGetDistance();
  simDigitalRead(0);
  simGetHumidity();
  simGetLuminosity();
  simGetTemperature();
}

// Fonction qui simule la distance entre deux seuils
// Elle interpolle la valeur de la distance à une fréquence
// étable avec rate à un pas donnée par step

// À utiliser comme si c'était un hc-sr04
// donc hc.getDistance() devient simGetDistance()
int simGetDistance() {
  static int startDistance = 50;  // Valeur initiale
  static int endDistance = 400;   // Valeur finale
  static int distance = startDistance; // Valeur retournée
  static int step = 3;  // Pas de l'interpolation
  static unsigned long previousTime = 0;
  static unsigned long rate = 250;  // Taux de rafraîchissement

  if (millis() - previousTime < rate) {
    return distance;
  }

  previousTime = millis();
  
  if (distance < startDistance || distance > endDistance) {
    step = -step;
  }

  distance += step;


  return distance;
}

// Fonction qui simule l'interrupteur
// À toutes les X secondes, elle change l'état de l'interrupteur
// À utiliser comme si c'était un bouton poussoir
// donc digitalRead(pin) devient simDigitalRead(pin)
int simDigitalRead(int pin) {
  static int state = 0;
  static unsigned long previousTime = 0;
  static unsigned long rate = 5000;  // Taux de rafraîchissement

  if (millis() - previousTime < rate) {
    return state;
  }

  previousTime = millis();

  state = !state;

  return state;
}


// Fonction qui simule un taux d'humidité entre deux seuils
// Elle interpole la valeur de l'humidité à une fréquence
// stable donnée par rate, avec un pas contrôlé par step.
//
// À utiliser comme si c'était un capteur d'humidité.
// Exemple : sensor.getHumidity() devient simGetHumidity()
//
// Retourne : la valeur simulée du taux d'humidité (en %)
int simGetHumidity() {
  static int startHumidity = 30;  // Valeur initiale
  static int endHumidity = 60;    // Valeur cible
  static int humidity = startHumidity;
  static int step = 1;       // Pas de l'interpolation
  static unsigned long previousTime = 0;
  static unsigned long rate = 250;  // Taux de rafraîchissement (ms)

  if (millis() - previousTime < rate) {
    return humidity;
  }

  previousTime = millis();

  if (humidity < startHumidity || humidity > endHumidity) {
    step = -step;
  }

  humidity += step;

  return humidity;
}

// Fonction qui simule le niveau de luminosité entre deux seuils
// Elle interpole la valeur de la luminosité à une fréquence
// stable donnée par rate, avec un pas contrôlé par step.
//
// À utiliser comme si c'était un capteur de luminosité.
// Exemple : sensor.getLuminosity() devient simGetLuminosity()
//
// Retourne : la valeur simulée du niveau de luminosité (de 0 à 1023)
int simGetLuminosity() {
  static int luminosity = 800;  // Valeur retournée
  static int startLuminosity = 200;  // Valeur initiale
  static int endLuminosity = 800;      // Valeur finale
  static int step = 10;         // Pas de l'interpolation
  static unsigned long previousTime = 0;
  static unsigned long rate = 250;  // Taux de rafraîchissement (ms)

  if (millis() - previousTime < rate) {
    return luminosity;
  }

  previousTime = millis();

  if (luminosity < startLuminosity || luminosity > endLuminosity) {
    step = -step;
  }

  luminosity += step;

  return luminosity;
}

// Fonction qui simule la température entre deux seuils
// Elle interpole la valeur de la température à une fréquence
// stable donnée par rate, avec un pas contrôlé par step.
//
// À utiliser comme si c'était un capteur de température.
// Exemple : sensor.getTemperature() devient simGetTemperature()
//
// Retourne : la valeur simulée de la température (en degrés Celsius)
int simGetTemperature() {
  static int temperature = 15;  // Valeur retournée
  static int startTemp = 15;  // Valeur initiale
  static int endTemp = 25;       // Valeur finale
  static int step = 1;          // Pas de l'interpolation
  static unsigned long previousTime = 0;
  static unsigned long rate = 250;  // Taux de rafraîchissement (ms)

  if (millis() - previousTime < rate) {
    return temperature;
  }

  previousTime = millis();

  if (temperature < startTemp || temperature > endTemp) {
    step = -step;
  }

  temperature += step;

  return temperature;
}

#pragma endregion