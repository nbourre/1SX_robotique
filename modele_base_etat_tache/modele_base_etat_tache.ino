
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