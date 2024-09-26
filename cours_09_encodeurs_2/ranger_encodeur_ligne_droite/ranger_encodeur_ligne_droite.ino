#include <MeAuriga.h>

#define BATT_PIN A4
#define BATTMAX 613  // 7.2v/12v * 1023

enum ModeState { AUTO,
                 MANUAL,
                 MAX_MODE_STATE };
                 
enum DriveState { IDLE,
                  FORWARD,
                  RIGHT,
                  MAX_DRIVE_STATE };

ModeState currentModeState = AUTO;
DriveState currentDriveState = IDLE;

unsigned long currentTime = 0;  // currentTime

MeEncoderOnBoard encoderLeft(SLOT1);
MeEncoderOnBoard encoderRight(SLOT2);


int battLevel = 0;

int ninetyDegree = 210;

int speed = 0;
bool speedNew = false;
int speedPrevious = 0;

// Variables pour les tâches

// Interval d'écriture dans le port série
unsigned long printPrevious = 0;
int printInterval = 250;

void isr_process_encoder1(void) {
  if (digitalRead(encoderLeft.getPortB()) == 0) {
    encoderLeft.pulsePosMinus();
  } else {
    encoderLeft.pulsePosPlus();
  }
}

void isr_process_encoder2(void) {
  if (digitalRead(encoderRight.getPortB()) == 0) {
    encoderRight.pulsePosMinus();
  } else {
    encoderRight.pulsePosPlus();
  }
}

void configureEncoders() {
  attachInterrupt(encoderLeft.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(encoderRight.getIntNum(), isr_process_encoder2, RISING);

  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  encoderLeft.setPulse(9);
  encoderRight.setPulse(9);
  encoderLeft.setRatio(39.267);
  encoderRight.setRatio(39.267);
  encoderLeft.setPosPid(1.8, 0, 1.2);
  encoderRight.setPosPid(1.8, 0, 1.2);
  encoderLeft.setSpeedPid(0.18, 0, 0);
  encoderRight.setSpeedPid(0.18, 0, 0);
}

void setup() {
  Serial.begin(115200);

  pinMode(BATT_PIN, INPUT);

  configureEncoders();
}

void loop() {
  currentTime = millis();

  serialInputTask();

  runState();

  encodersTask();

  serialOutputTask(currentTime);
}

int getBattLevel() {
  int value = analogRead(A4);
  return (int)((value * 100.0) / BATTMAX);
}

void runState() {
  switch (currentModeState) {
    case AUTO:
      autoState();
      break;
    case MANUAL:
      manualState();
      break;
  }
}

void manualState() {
}

bool taskBegin = true;

void forwardState(unsigned long ct) {
  unsigned long firstTime = true;
  unsigned long exitTime = 0;
  unsigned int rate = 3000;

  // Entrée dans l'état
  if (firstTime) {
    firstTime = false;
    exitTime = ct + rate;

    encoderLeft.runSpeed(-100);
    encoderRight.runSpeed(100);

    Serial.println("Forward");
    Serial.print("Left pos :");
    Serial.println(encoderLeft.getPulsePos());
  }

  // Code de l'état
  // Inutile dans le cas du moteur
  // car les valeurs sont persistentes


  // Sortie de l'état
  bool transition = ct >= exitTime;

  if (transition) {
    firstTime = true;
    currentDriveState = RIGHT;
  }
}

void idleState(unsigned long ct) {
  unsigned long firstTime = true;
  unsigned long exitTime = 0;
  unsigned int rate = 3000;

  // Entrée dans l'état
  if (firstTime) {
    firstTime = false;
    exitTime = ct + rate;

    speed = 0;
    speedNew = true;

    Serial.println("Idle");
  }

  // Code de l'état
  // Inutile dans le cas du moteur
  // car les valeurs sont persistentes


  // Sortie de l'état
  bool transition = ct >= exitTime;

  if (transition) {
    firstTime = true;
    currentDriveState = RIGHT;
  }
}


void rightState(unsigned long ct) {
  unsigned long firstTime = true;
  unsigned long exitTime = 0;
  unsigned int rate = 3000;

  // Entrée dans l'état
  if (firstTime) {
    firstTime = false;
    exitTime = ct + rate;

    encoderLeft.move(ninetyDegree, 255);
    encoderRight.move(ninetyDegree, 255);

    Serial.println("Right");
  }

  // Code de l'état
  // Inutile dans le cas du moteur
  // car les valeurs sont persistentes


  // Sortie de l'état
  bool transition = ct >= exitTime;

  if (transition) {
    firstTime = true;
    currentDriveState = RIGHT;
  }
}

void autoState() {

  switch (currentDriveState) {
    case FORWARD:
      forwardState(currentTime);
      break;
    case IDLE:
      idleState(currentTime);
      break;
    case RIGHT:
      rightState(currentTime);

      break;
    default:
      break;
  }
}

// Affiche l'état actuel
void printDriveState() {
  switch (currentDriveState) {
    case FORWARD:
      Serial.println("Forward");
      break;
    case IDLE:
      Serial.println("Idle");
      break;
    case RIGHT:
      Serial.println("Turning right");
      break;
    default:
      break;
  }
}


// Fonction qui regarde s'il y a eu une
// entrée via le port série
void serialInputTask() {
  if (Serial.available()) {
    char a = Serial.read();

    // On regarde si c'est la valeur et entre 0 et 6
    if (a >= '0' && a <= '6') {
      // On active le drapeau pour indiquer 
      // une nouvelle vitesse
      speedNew = true;

      if (a > '0') {
        currentModeState = MANUAL;
        Serial.println("Mode manuel activé");
      }
    }

    switch (a) {
      case '0':
        speed = 0;
        currentModeState = AUTO;
        currentDriveState = IDLE;
        Serial.println("Mode automatique activé");
        break;
      case '1':
        speed = -100;
        break;
      case '2':
        speed = -200;
        break;
      case '3':
        speed = -255;
        break;
      case '4':
        speed = 100;
        break;
      case '5':
        speed = 200;
        break;
      case '6':
        speed = 255;
        break;
      case 'b':
        Serial.print("Battery level : ");
        Serial.println(getBattLevel());
        break;
      default:
        break;
    }
  }
}

// Le moteur tourne tjrs, alors je les ai
// converti en tâche
void encodersTask() {

  if (speedNew) {
    encoderLeft.runSpeed(-speed);
    encoderRight.runSpeed(speed);
    speedNew = false;
  }

  encoderLeft.loop();
  encoderRight.loop();
}

void serialOutputTask(unsigned long ct) {
  return; // J'ignore la fonction
  
  unsigned long firstTime = true;
  unsigned long lastTime = 0;
  unsigned int rate = 250;
  
  if (ct - lastTime < rate) return;

  lastTime = ct;

  Serial.print("Speed 1:");
  Serial.print(encoderLeft.getCurrentSpeed());
  Serial.print(" ,Speed 2:");
  Serial.println(encoderRight.getCurrentSpeed());
}
