// Light Sensor
#define LSL A3
#define LSR A2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LSL, INPUT);
  pinMode(LSR, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int droite = analogRead(LSR);
  int gauche = analogRead(LSL);

  Serial.print("LSL:");
  Serial.print(gauche);
  Serial.print("\tLSR:");
  Serial.print(droite);
  Serial.print("\tTend vers la : ");
  Serial.println(droite - gauche > 0 ? "droite" : "gauche");

  delay(50);
}
