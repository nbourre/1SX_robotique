const int MaxChars = 5; // un int c'est max 5 car
char strValue[MaxChars+1]; // doit être assez gros + char null
int index = 0;         // Index sauvegarder les chiffres
int   blinkRate=1000; // taux de rafraichissement sauvegardé

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  if( Serial.available())
  {
    char ch = Serial.read();
    if(index <  MaxChars && ch >= '0' && ch <= '9'){
      strValue[index++] = ch; // ajouter le ASCII au tableau
    }
    else
    {
      // Premier car non reconnu ou buffer full
      strValue[index] = 0; // Terminer le string avec 0 (null)
      blinkRate = atoi(strValue);  // Utiliser atoi pour convertir
      index = 0;
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