// Demonstrate how to use the gyro and encoders to go straight
// using a PD controller to correct the angle
// The application use a simple state machine to change the state
// Author: Nicolas Bourré

#include <MeAuriga.h>

enum AppState {STOP, STRAIGHT, TURNING};

AppState currentState = STOP;

#define DIST_WHEEL 151
#define DIA_WHEEL 64.5
#define PULSE 9
#define RATIO 39.267
#define FULL_TURN_CIRC 948.8
#define FULL_SPIN_CIRC 474.4
#define CIRC_WHEEL 202.6

MeGyro gyro(0, 0x69);

MeEncoderOnBoard encoderRight(SLOT1);
MeEncoderOnBoard encoderLeft(SLOT2);

unsigned long currentTime = 0;

unsigned long serialPrintPrevious = 0;
int serialPrintInterval = 500;
String msg = "";


// ********* INTERRUPTIONS ***********

void rightEncoderInterrupt(void)
{
  if(digitalRead(encoderRight.getPortB()) == 0)
  {
    encoderRight.pulsePosMinus();
  }
  else
  {
    encoderRight.pulsePosPlus();;
  }
}

void leftEncoderInterrupt(void) {
  if(digitalRead(encoderLeft.getPortB()) == 0)
  {
    encoderLeft.pulsePosMinus();
  }
  else
  {
    encoderLeft.pulsePosPlus();
  }
}

// ************* DÉBUT ************

void encoderConfig() {
  attachInterrupt(encoderRight.getIntNum(), rightEncoderInterrupt, RISING);
  attachInterrupt(encoderLeft.getIntNum(), leftEncoderInterrupt, RISING);
  
  encoderRight.setPulse(PULSE);
  encoderLeft.setPulse(PULSE);
  
  encoderRight.setRatio(RATIO);
  encoderLeft.setRatio(RATIO);
  
  encoderRight.setPosPid(1.8,0,1.2);
  encoderLeft.setPosPid(1.8,0,1.2);
  
  encoderRight.setSpeedPid(0.18,0,0);
  encoderLeft.setSpeedPid(0.18,0,0);
  
  // DÉBUT : Ne pas modifier ce code!
  // Configuration de la fréquence du PWM
  // Copier-coller ce code si on désire
  // travailler avec les encodeurs
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  // FIN : Ne pas modifier ce code!
}


void setup() {
  Serial.begin(115200);
  encoderConfig();
  gyro.begin();
  
  // Waiting 3 sec before start
  Serial.println("Waiting 3 sec");
  delay (3000);
  straightCmd();
}

void loop() {
  currentTime = millis();
  
  switch (currentState) {
    case STOP:
      stopState();
      break;
    case STRAIGHT:
      straightState();
      break;
    default:
      stopState();
      break;
  }
  
  gyroTask(currentTime);
  encodersTask(currentTime);
  serialPrintTask(currentTime);
}

void gyroTask(unsigned long ct) {
  gyro.update();
}

void encodersTask(unsigned long ct) {
  encoderRight.loop();
  encoderLeft.loop();
}


void goStraight(short speed = 100, short firstRun = 0) {
    static double zAngleGoal = 0.0;
    
    static double error = 0.0;
    static double previousError = 0.0;
    static double output = 0;
    
    // PD Controller
    // Change these values to suit your needs
    // higher kp = more reactive, might have oscillation
    // lowewr kp = sluggish, but less oscillation
    // higher kd = limit oscillation, the right value stops oscillation
    const double kp = 3.0;
    const double kd = 1.0;    
    
    if (firstRun) {
      zAngleGoal = gyro.getAngleZ();
      firstRun = 0;
      Serial.println ("Setting speed");
      
      encoderLeft.setTarPWM(speed);
      encoderRight.setTarPWM(-speed);
      
      return;
    }
    
    error = gyro.getAngleZ() - zAngleGoal;
    
    // Google : PID
    output = kp * error + kd * (error - previousError);
    
    previousError = error;        
    
    msg = "z : ";
    msg += gyro.getAngleZ();
    msg += "\tleft : ";
    msg += encoderLeft.getCurPwm();
    msg += "\tright : ";
    msg += encoderRight.getCurPwm();
    msg += "\toutput :";
    msg += output;
    
    encoderLeft.setTarPWM(speed - output);
    encoderRight.setTarPWM(-speed - output);
    
}

void stopCmd() {
  Serial.println ("Stopping");
  msg = "";
  
  currentState = STOP;
}

void straightCmd() {
  Serial.println ("Going straight");
 
  currentState = STRAIGHT;
  goStraight(100, 1);
}

void stopState() { 
  encoderLeft.setTarPWM(0);
  encoderRight.setTarPWM(0);
}

void straightState() {  
  goStraight();
}

void serialEvent() {
  while (Serial.available()) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case '0':
        stopCmd();
        break;
      case '1':
        straightCmd();
        break;
    }
  }
}

void serialPrintTask(unsigned long cT) {
  static unsigned long lastTime = 0;
  const int rate = 500;
  
  if (cT - lastTime < rate) return;

  lastTime = cT;

  if (msg != "") {
    Serial.println(msg);
    msg = "";
  }
}