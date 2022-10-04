/**
 * @file         ranger_moteur_sans_librairie
 * @author       Nicolas Bourré
 * @version      V1.0
 * @date         2022/10/03
 * @description  this file is sample code for the mBot Ranger kit
 */

//enum State {DRIVING, TURN, STOP, MAX_STATE};
enum State {DRIVING, STOP, MAX_STATE};

State currentState = STOP;

long currentTime = 0;

long statePrevious = 0;
int stateDelay = 3000;

int maxPwm = 255;

//Motor right
const int m1_pwm = 11;
const int m1_IN2 = 49; // M1 ENB
const int m1_IN1 = 48; // M1 ENA

void setup() {
  Serial.begin(9600);

  pinMode(m1_pwm, OUTPUT);   //We have to set PWM pin as output
  pinMode(m1_IN2, OUTPUT);  //Logic pins are also set as output
  pinMode(m1_IN1, OUTPUT);

  currentTime = millis();
}

void loop() {
  currentTime = millis();

  switch (currentState){
    case DRIVING:
      ReduceSpeed();
      break;
    case STOP:
      Stop();      
      break;
    default:
      currentState = STOP;
  }

  if (currentTime - statePrevious >=  stateDelay) {
    statePrevious = currentTime;    
    
    currentState = (currentState + 1) % MAX_STATE;
    Serial.print ("Entering state : ");
    printState();
  }
}

void printState() {
  switch (currentState){
    case DRIVING:
      Serial.println ("DRIVING");
      break;
    case STOP:
      Serial.println ("STOPPED");
      break;
    default:
      Serial.println ("UNKNOWNED STATE");
  }  
}

void FullSpeedMode() {
  digitalWrite(m1_IN2, LOW);
  digitalWrite(m1_IN1, HIGH);
  analogWrite(m1_pwm, maxPwm);
}

void ReduceSpeed() {
  digitalWrite(m1_IN2, LOW);
  digitalWrite(m1_IN1, HIGH);
  analogWrite(m1_pwm, maxPwm >> 1);  //Set speed via PWM
}

void Stop() {
  analogWrite(m1_pwm, 0);
}

void TurnRight() {
  digitalWrite(m1_IN2, LOW);
  digitalWrite(m1_IN1, HIGH);
  analogWrite(m1_pwm, maxPwm >> 1);  //Set speed via PWM
}