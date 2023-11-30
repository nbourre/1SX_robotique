#include <MeAuriga.h>

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
  runner.execute();
}

void goStraightCallback() {
  static float currentAngle = 0.0f;
  if (taskGoStraight.isFirstIteration()) {
    currentAngle = 
  }
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
