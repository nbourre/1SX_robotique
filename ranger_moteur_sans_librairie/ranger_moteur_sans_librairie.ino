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
int halfPwm = 125;
int turnPwm = 150;


//Motor Left
const int m1_pwm = 11;
const int m1_in1 = 48; // M1 ENA
const int m1_in2 = 49; // M1 ENB

//Motor Right
const int m2_pwm = 10;
const int m2_in1 = 47; // M2 ENA
const int m2_in2 = 46; // M2 ENB

void setup() {
  Serial.begin(9600);

  pinMode(m1_pwm, OUTPUT);   //We have to set PWM pin as output
  pinMode(m1_in2, OUTPUT);  //Logic pins are also set as output
  pinMode(m1_in1, OUTPUT);

  pinMode(m2_pwm, OUTPUT);   //We have to set PWM pin as output
  pinMode(m2_in2, OUTPUT);  //Logic pins are also set as output
  pinMode(m2_in1, OUTPUT);

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
  digitalWrite(m1_in2, LOW);
  digitalWrite(m1_in1, HIGH);
  analogWrite(m1_pwm, maxPwm);

  digitalWrite(m2_in2, HIGH);
  digitalWrite(m2_in1, LOW);
  analogWrite(m2_pwm, maxPwm);
}

void ReduceSpeed() {
  digitalWrite(m1_in2, LOW);
  digitalWrite(m1_in1, HIGH);
  analogWrite(m1_pwm, halfPwm);  //Set speed via PWM
  
  digitalWrite(m2_in2, HIGH);
  digitalWrite(m2_in1, LOW);
  analogWrite(m2_pwm, halfPwm);     //Set speed via PWM
}

void Stop() {
  analogWrite(m1_pwm, 0);
  analogWrite(m2_pwm, 0);
  Serial.println("Stop");
}

void TurnRight() {
  digitalWrite(m1_in2, LOW);
  digitalWrite(m1_in1, HIGH);
  analogWrite(m1_pwm, turnPwm);  //Set speed via PWM
  
  digitalWrite(m2_in2, LOW);
  digitalWrite(m2_in1, HIGH);
  analogWrite(m2_pwm, turnPwm);         //Set speed via PWM
}