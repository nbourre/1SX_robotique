
#include <Servo.h>

int ledPin = 10;
Servo servo;
int servoPin = 7;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  pinMode (ledPin, OUTPUT);
  servo.attach(servoPin);
}

int ledValue = 0;
int servoValue = 0;

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  ledValue = map(sensorValue, 0, 1023, 0, 255);
  servoValue = map(sensorValue, 0, 1023, 180, 0);
  
  analogWrite(ledPin, ledValue);
  servo.write(servoValue);


  // print out the value you read:
  //Serial.println(sensorValue);
  delay(10);        // delay in between reads for stability
}
