
unsigned long currentTime = 0;
    
void setup() {
  Serial.begin(115200);

}

void loop() {
  currentTime = millis();

}

void modeleState(unsigned long ct)
{
    static unsigned long previousTime = ct;
    static unsigned long rate = 1000; // 1 seconde
    static bool firstTime = true;


    if (firstTime)
    {
        firstTime = false;
        // Code à exécuter une seule fois au début
        // Initialisation des variables
        Serial.println("Entrée État : Modèle");
    }

    // Si nécessaire
    if (ct - previousTime < rate)
    {
        return;
    }

    previousTime = ct;

    // Code à exécuter à chaque appel de la fonction
    // Mise à jour des variables

    bool transition = false; // Mettre à vrai si on change d'état

    if (transition)
    {
        // Code à exécuter une seule fois à la fin
        // Nettoyage des variables
        Serial.println("Sortie État : Modèle");

        firstTime = true;
    }

    // Vous pouvez ajouter d'autres transitions ici
}

void modeleTask(unsigned long ct)
{
    static unsigned long previousTime = 0;
    static unsigned long rate = 1000; // 1 seconde

    if (ct - previousTime < rate)
    {
        return;
    }

    previousTime = ct;

    // Code à exécuter à chaque appel de la fonction
    // Mise à jour des variables
}

// Fonction qui simule la distance entre deux seuils
// Elle interpolle la valeur de la distance à une fréquence
// étable avec rate à un pas donnée par step

// À utiliser comme si c'était un hc-sr04
// donc hc.getDistance() devient simGetDistance()
int simGetDistance() {
    static int distance = 5;    // Valeur initiale
    static int target = 25;     // Valeur cible
    static int step = 1;        // Pas de l'interpolation
    static unsigned long previousTime = 0;
    static unsigned long rate = 250; // Taux de rafraîchissement

    if (millis() - previousTime < rate)
    {
        return distance;
    }

    previousTime = millis();

    if (distance < target)
    {
        step = 1;
    }
    else if (distance > target)
    {
        step = -1;
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
    static unsigned long rate = 5000; // Taux de rafraîchissement

    if (millis() - previousTime < rate)
    {
        return state;
    }

    previousTime = millis();

    state = !state;

    return state;
}
