int ledPin = 13;

unsigned long cT;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  analogWrite(ledPin, 10) ;
}