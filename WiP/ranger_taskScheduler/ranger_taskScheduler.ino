
unsigned long currentTime = 0;

#pragma region ROBOT DECLARATION & INITIALIZATION
#include <MeAuriga.h>

MeEncoderOnBoard encoderRight(SLOT1);
MeEncoderOnBoard encoderLeft(SLOT2);
MeGyro gyro(0, 0x69);

int speed = 100;

void isr_process_encoderRight(void)
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

void isr_process_encoderLeft(void)
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

void initEncoders() {
  attachInterrupt(encoderRight.getIntNum(), isr_process_encoderRight, RISING);
  attachInterrupt(encoderLeft.getIntNum(), isr_process_encoderLeft, RISING);

  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
}

#pragma endregion

#pragma region TASKSCHEDULER DECLARATION
// Définir les options de TaskScheduler si requis
#include <TaskScheduler.h>

// Déclarations des tâches
void goStraightCallback();
void detectObjectCallback();
void emitSoundCallback();
void blinkLEDCallback();
void pivotCallback();

// Création des objets Task
Task taskGoStraight(TASK_IMMEDIATE, TASK_FOREVER, &goStraightCallback);
Task taskDetectObject(100, TASK_FOREVER, &detectObjectCallback);
Task taskEmitSound(1000, TASK_ONCE, &emitSoundCallback);
Task taskBlinkLED(2000, TASK_ONCE, &blinkLEDCallback, NULL, false, &pivotCallback);
Task taskPivot(TASK_IMMEDIATE, TASK_ONCE, &pivotCallback);

Scheduler runner;

#pragma endregion

#pragma region SETUP AND LOOP

void setup() {
  runner.init();

  // Ajout des tâches au planificateur
  runner.addTask(taskGoStraight);
  runner.addTask(taskDetectObject);
  runner.addTask(taskEmitSound);
  runner.addTask(taskBlinkLED);
  runner.addTask(taskPivot);

  // Activation des tâches initiales
  taskGoStraight.enable();
  taskDetectObject.enable();

  runner.startNow();
}

void loop() {
  currentTime = millis();

  encoderRight.loop();
  encoderLeft.loop();
  gyro.update();

  runner.execute();
}

#pragma endregion

#pragma region CALLBACKS

void goStraightCallback() {
  static double currentAngle = 0.0f;
  static double angleGoal = 0.0f;
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

  if (taskGoStraight.isFirstIteration()) {
    currentAngle = gyro.getAngleZ();
    angleGoal = gyro.getAngleZ();
    
    encoderLeft.setMotorPwm(speed);
    encoderRight.setMotorPwm(-speed);
  }

  error = gyro.getAngleZ() - angleGoal;
  
  // Google : ELI5 PID
  // Astuce web : ELI5 = Explain Like I'm 5
  output = kp * error + kd * (error - previousError);
  
  previousError = error;        
  
  
  encoderLeft.setTarPWM(speed - output);
  encoderRight.setTarPWM(-speed - output);
}

void detectObjectCallback() {
  // Code pour détecter une collision
  int hasCollision = 0;
  if (hasCollision) {
    taskGoStraight.disable();
    taskDetectObject.disable();
    taskEmitSound.enable();
    taskBlinkLED.enable();
  }
}

void emitSoundCallback() {
  // Code pour émettre un son pendant 1 seconde
  taskEmitSound.disable();
}

void blinkLEDCallback() {
  // Code pour faire clignoter une LED à 10 Hz pendant 2 secondes
  // La tâche suivante (taskPivot) est déclenchée à la fin de cette tâche
}

void pivotCallback() {
  static float currentZ = 0.0;
  // Code pour pivoter de 120 degrés

  if (taskPivot.isFirstIteration()) {

  }

  int done = 0;

  if (done) {
    taskGoStraight.enable();
    taskDetectObject.enable();
  }
}

#pragma endregion
