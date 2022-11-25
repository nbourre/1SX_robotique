
unsigned long currentTime = 0;

enum State {BOOTING, CALIBRATION, NORMAL, MAX_STATE};

State currentState = BOOTING;

byte soundPin = A1;
unsigned long soundSum = 0;
int soundSamples = 1024;
int soundBase = 0;
int soundSampleCount = 0;
bool soundDone = false;

int soundDiff = 0;

byte leftSensor = A3;
byte rightSensor = A2;
unsigned long leftSum = 0;
unsigned long rightSum = 0;
int leftBase = 0;
int rightBase = 0;
int lightSampleCount = 0;
int lightSamples = 1024;
bool lightDone = false;
int leftVal = 0;
int rightVal = 0;

unsigned long serialPrintPrevious = 0;
int serialPrintInterval = 100;

void calibrate() {
  calibrate_sound();
  calibrate_light();

  if (lightDone && soundDone) {
    currentState = NORMAL;
  }
}

void calibrate_sound() {
  if (soundDone) return;

  soundSum += analogRead(soundPin);
  soundSampleCount++;

  if (soundSampleCount >= soundSamples) {
    soundBase = soundSum >> 10; // Division par 1024
    soundDone = true;
  }
}

void calibrate_light() {
  if (lightDone) return;
  
  leftSum += analogRead(leftSensor);
  rightSum += analogRead(rightSensor);

  lightSampleCount++;

  if (lightSampleCount >= lightSamples) {
    leftBase = leftSum / lightSampleCount;
    rightBase = rightSum / lightSampleCount;
    lightDone = true;
  }
}

void calibrate_enter() {
  currentState = CALIBRATION;
  soundSum = 0;
  soundSampleCount = 0;
  soundBase = 0;

  leftSum = 0;
  rightSum = 0;
  leftBase = 0;
  rightBase = 0;
  lightSampleCount = 0;
  
  lightDone = false;
  soundDone = false;
}

void lightTest() {
  leftVal = analogRead(leftSensor);
  rightVal = analogRead(rightSensor);
}

void soundTest() {
  soundDiff = analogRead(soundPin) - soundBase;
  
}

void run() {
 soundTest();
 lightTest();
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
  Serial.print(soundBase);
  Serial.print(",soundDiff:");
  Serial.print(soundDiff);
  Serial.print(",leftBase:");
  Serial.print(leftBase);
  Serial.print(",rightBase:");
  Serial.print(rightBase);
  Serial.print(",leftValue:");
  Serial.print(leftVal);
  Serial.print(",rightValue:");
  Serial.println(rightVal);
}
