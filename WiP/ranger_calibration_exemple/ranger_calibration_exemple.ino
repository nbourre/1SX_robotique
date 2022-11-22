
unsigned long currentTime = 0;

enum State {BOOTING, CALIBRATION, NORMAL, MAX_STATE};

State currentState = BOOTING;

byte soundPin = A1;
unsigned long soundSum = 0;
int soundSamples = 1024;
int soundBase = 0;
int sampleCount = 0;

unsigned long serialPrintPrevious = 0;
int serialPrintInterval = 2000;


void calibrate() {
  soundSum += analogRead(soundPin);
  sampleCount++;

  if (sampleCount >= soundSamples) {
    soundBase = soundSum >> 10; // Division par 1024
    currentState = NORMAL;
  }
}

void calibrate_enter() {
  currentState = CALIBRATION;
  soundSum = 0;
  sampleCount = 0;
  soundBase = 0;
}

void run() {

}

void setup() {
  Serial.begin(115200);
 
  calibrate_enter();
}

void loop() {
  currentTime = millis();
  // put your main code here, to run repeatedly:

  switch (currentState) {
    case CALIBRATION:
      calibrate();
      break;
    case NORMAL:
      run();
      break;
  }

  serialPrintTask();

}

void serialPrintTask() {
  if (currentTime - serialPrintPrevious < serialPrintInterval) return;

  serialPrintPrevious = currentTime;

  Serial.print("soundBase:");
  Serial.println(soundBase);
  
}
