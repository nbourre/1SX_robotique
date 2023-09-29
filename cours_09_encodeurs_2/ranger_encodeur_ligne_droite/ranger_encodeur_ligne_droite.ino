#include <MeAuriga.h>

#define BATTMAX 613 // 7.2v/12v * 1023

enum ModeState {AUTO, MANUAL, MAX_MODE_STATES};
enum DriveState {IDLE, FORWARD, RIGHT, MAX_DRIVE_STATE};

ModeState currentModeState = AUTO;
DriveState currentDriveState = IDLE;

unsigned long currentTime = 0; // currentTime

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

// Interval de changement d'état
// pour le mode automatique
unsigned long autoPrevious = 0;
int autoInterval = 3000;

void isr_process_encoder1(void)
{
  if(digitalRead(encoderLeft.getPortB()) == 0)
  {
    encoderLeft.pulsePosMinus();
  }
  else
  {
    encoderLeft.pulsePosPlus();
  }
}

void isr_process_encoder2(void)
{
  if(digitalRead(encoderRight.getPortB()) == 0)
  {
    encoderRight.pulsePosMinus();
  }
  else
  {
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
  encoderLeft.setPosPid(1.8,0,1.2);
  encoderRight.setPosPid(1.8,0,1.2);
  encoderLeft.setSpeedPid(0.18,0,0);
  encoderRight.setSpeedPid(0.18,0,0);
}

void setup()
{
  Serial.begin(115200);
  
  pinMode(A4, INPUT);
  
  configureEncoders();
}

void loop()
{
  currentTime = millis();
  
  serialInputTask();
  
  runState();
  
  encodersTask();
  
  serialOutputTask();
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

void forwardTask() { 
  if (taskBegin) {
    Serial.print("Left pos :");
    Serial.println(encoderLeft.getPulsePos());
    taskBegin = false;
    encoderLeft.runSpeed(-100);
    encoderRight.runSpeed(100);
  }
}

void idleTask() {
  if (taskBegin) {
    speed = 0;
    speedNew = true;
    taskBegin = false;
  }    
}


void rightTask() {
  if (taskBegin) {
    encoderLeft.move(ninetyDegree, 255);
    encoderRight.move(ninetyDegree, 255);
    taskBegin = false;
  }  
}

void autoState() {
  
  switch (currentDriveState) {
    case FORWARD:
      forwardTask();
      break;
    case IDLE:
      idleTask();
      break;
    case RIGHT:
      rightTask();
      
      break;
    default:
      break;    
  }
  
  if (currentTime - autoPrevious >= autoInterval) {
    autoPrevious = currentTime;
    currentDriveState = (currentDriveState + 1) % MAX_DRIVE_STATE;
    
    printDriveState();
    
    // Drapeau pour indiquer que l'on débute une nouvelle tâche
    taskBegin = true;    
  }
  
}

void printDriveState()
{
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



void serialInputTask() {
  if(Serial.available())
  {
    char a = Serial.read();
    
    // On regarde si c'est la valeur et entre 0 et 6
    if (a >= '0' && a <= '6') {
      speedNew = true;
      
      if (a > '0') {
        currentModeState = MANUAL;
        Serial.println("Mode manuel activé");
      }
    }
    
    switch(a)
    {
      case '0':
        speed = 0;
        currentModeState = AUTO;
        currentDriveState = IDLE;
        autoPrevious = currentTime;
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

void encodersTask() {
  
  if (speedNew) {
    encoderLeft.runSpeed(-speed);
    encoderRight.runSpeed(speed);
    speedNew = false;
  }
  
  encoderLeft.loop();
  encoderRight.loop();
}

void serialOutputTask() {
  return;
  if (currentTime - printPrevious < printInterval) return;
  
  printPrevious = currentTime;
  
  Serial.print("Speed 1:");
  Serial.print(encoderLeft.getCurrentSpeed());
  Serial.print(" ,Speed 2:");
  Serial.println(encoderRight.getCurrentSpeed());  
}


