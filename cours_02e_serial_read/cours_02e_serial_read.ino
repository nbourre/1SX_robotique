int   blinkRate=0; // taux de rafraichissement sauvegardé

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  if ( Serial.available()) // Vérifier si l'on a au moins 1 octet de dispo
  {
    char ch = Serial.read(); // Lire le prochain octet
    if(ch >= '0' && ch <= '9') // ESt-ce que c'est une valeur entre '0' et '9'
    {
       blinkRate = (ch - '0');      // Valeur ASCII converti en numérique
       blinkRate = blinkRate * 100; // Interval

      Serial.print("Reçu : ");
      Serial.println(ch);
    }

  }
  blink();
}

// Faire clignoter le LED
void blink()
{
  digitalWrite(LED_BUILTIN,HIGH);
  delay(blinkRate);
  digitalWrite(LED_BUILTIN,LOW);
  delay(blinkRate);
}