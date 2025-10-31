#include <Arduino.h>
#include <MeAuriga.h>
#include <Adafruit_seesaw.h>

#define CAPTEUR_COUNT 5

Adafruit_seesaw ss;

enum AppState {
  IDLE,
  CALIBRATION,
  RUNNING
};

AppState currentState = IDLE;


struct Capteur {
  uint16_t val;
  uint16_t valMin;
  uint16_t valMax;
  uint16_t seuil;
};


Capteur capteurs[CAPTEUR_COUNT];

bool debugMode = false;

// SERIAL COMMANDS
bool serialCommandFlag = false;
String serialCommand = "";


// PROTOTYPES

// TASKS
void calibrationCapteursTask(unsigned long ct);
void lireCapteursTask(unsigned long ct);
void serialPrintTask(unsigned long ct);


// STATES


// HELPERS
void manageSerialData();
void handleCommand(String command);
void handleCommandWithParams(String command, String params);


#pragma region TASKS

void lireCapteursTask(unsigned long ct) {
  static unsigned long lastTime = 0;
  const unsigned int rate = 10; // ms

  if (ct - lastTime < rate) {
    return;
  }

  for (int i = 0; i < CAPTEUR_COUNT; i++) {
    capteurs[i].val = ss.analogRead(i);
  }
}

void calibrationCapteursTask(unsigned long ct) {
  static unsigned long lastTime = 0;
  const unsigned int rate = 10; // ms

  if (ct - lastTime < rate) {
    return;
  }

  for (int j = 0; j < CAPTEUR_COUNT; j++) {
    if (capteurs[j].val < capteurs[j].valMin) {
      capteurs[j].valMin = capteurs[j].val;
      
    }
    if (capteurs[j].val > capteurs[j].valMax) {
      capteurs[j].valMax = capteurs[j].val;
    }
    capteurs[j].seuil = (capteurs[j].valMin + capteurs[j].valMax) / 2;
  }
}




void serialPrintTask(unsigned long ct) {
  static unsigned long lastTime = 0;
  const unsigned int rate = 1000; // ms

  if (ct - lastTime < rate) {
    return;
  }
  lastTime = ct;

  Serial.println("Capteurs:");
  for (int i = 0; i < CAPTEUR_COUNT; i++) {
    Serial.print("Capteur ");
    Serial.print(i);
    Serial.print(":");
    Serial.print(capteurs[i].val);
    Serial.print("\t");
  }
  Serial.println();
}

#pragma endregion TASKS

#pragma region STATES

void templateState(unsigned long ct) {
  static bool firstTime = true;

  static unsigned long lastTime = 0;
  const unsigned int rate = 100; // ms

  if (firstTime) {
    firstTime = false;
    
  }

  if (ct - lastTime < rate) {
    return;
  }

  lastTime = ct;

  // DO STUFF HERE

  bool transitionCondition = false;

  if (transitionCondition) {
    firstTime = true;
    currentState = IDLE; // Changer l'état
    return;
  }

}

#pragma endregion STATES

#pragma region SERIAL_COMMANDS_HANDLERS

void serialEvent() {
  if (!Serial.available()) {
    return;
  }

  serialCommand = Serial.readStringUntil('\n');
  serialCommand.trim();
  serialCommand.replace("\r", ""); // Supprimer les retours chariot éventuels

  serialCommandFlag = true;
}

void manageSerialData() {
  if (!serialCommandFlag) {
    return;
  }

  bool isFromBle = false;

  serialCommandFlag = false;

  if (serialCommand.length() >= 2) {
    if ((uint8_t)serialCommand[0] == 0xFF && (uint8_t)serialCommand[1] == 0x55) {
      isFromBle = true;
      // Supprimer les deux premiers caractères
      serialCommand = serialCommand.substring(2);
    }
    else if (serialCommand.startsWith("!!")) {
      // Provient du moniteur série
      // Supprimer les deux premiers caractères
      serialCommand = serialCommand.substring(2);
    }
    else {
      Serial.println(F("Entête de commande invalide"));
      Serial.print(F("Reçu: "));
      Serial.println(serialCommand);
      return;
    }
  }
  else {
    Serial.println(F("Commande invalide"));
    Serial.print(F("Reçu: "));
    Serial.println(serialCommand);
    return;
  }

  if (debugMode) {
    Serial.print(F("Commande reçue: "));
    Serial.println(serialCommand);
    Serial.print(F("Provenance: "));
    Serial.println(isFromBle ? F("BLE") : F("Série"));
  }

  // Découpage de la commande et des paramètres
  int firstCommaIndex = serialCommand.indexOf(',');

  if (firstCommaIndex == -1) {
    // Commande sans paramètres
    handleCommand(serialCommand);
  }
  else {
    // Commande avec paramètres
    String command = serialCommand.substring(0, firstCommaIndex);
    String params = serialCommand.substring(firstCommaIndex + 1);
    handleCommandWithParams(command, params);
  }
}


void handleCommand(String command) {
  command.toUpperCase();
  if (command == "DEBUG") {
    debugMode = !debugMode;
    Serial.println(debugMode ? F("Mode debug activé") : F("Mode debug désactivé"));
  }
  else if (command == "CALIB") {
    currentState = CALIBRATION;
  }
  else {
    Serial.print(F("Commande inconnue: "));
    Serial.println(command);
  }
}

void handleCommandWithParams(String command, String params) {

}

#pragma endregion SERIAL_COMMANDS_HANDLERS

#pragma region SETUP_LOOP

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < CAPTEUR_COUNT; i++) {
    capteurs[i].val = 0;
    capteurs[i].valMin = 1023;
    capteurs[i].valMax = 0;
    capteurs[i].seuil = 0;
  }

  if (!ss.begin()) {
    Serial.println("seesaw not found!");
    while (1) delay(10);
  }

  Serial.println("seesaw found!");
}

void loop() {
  unsigned long currentTime = millis();

  manageSerialData();

  lireCapteursTask(currentTime);
  calibrationCapteursTask(currentTime);




  serialPrintTask(currentTime);

}

#pragma endregion SETUP_LOOP