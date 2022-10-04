int ledPin = 10;
unsigned long cT = 0;
int pwm = 10;
int dirIncrement = 5; // Direction de l'incrément

unsigned long pwmPrevious = 0;
int pwmDelay = 20;

void setup() {
  // put your setup code here, to run once:
  pinMode (ledPin, OUTPUT);
  cT = millis();
}

void loop() {
  cT = millis();

  if (cT - pwmPrevious >= pwmDelay) {
    pwmPrevious = cT;

    if (pwm <= 0 || pwm >= 255) {
      dirIncrement = -dirIncrement;
    }

    pwm += dirIncrement;
  }
  
  analogWrite(ledPin, pwm);

}
