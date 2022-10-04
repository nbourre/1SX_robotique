#include <DHT.h>

// Broche de données
#define DHTPIN 2

// Type de capteur pour la librairie
#define DHTTYPE DHT11  // DHT 11

// Déclaration de l'objet
DHT dht(DHTPIN, DHTTYPE);

long currentTime = 0;
long dhtPrevious = 0;
long dhtDelay = 1000;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();  // Initialisation

  currentTime = millis();
}

void loop() {
  currentTime = millis();

  if (currentTime - dhtPrevious >= dhtDelay) {
    dhtPrevious = currentTime;

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float humidex = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidité: ");
    Serial.print(h);
    Serial.print(("%  Température: "));
    Serial.print(t);
    Serial.print(("° C "));
    Serial.print(("Humidex : "));
    Serial.print(humidex);
    Serial.println(("° C "));
  }
}