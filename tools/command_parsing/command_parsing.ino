
enum AppState {STOP, FOLLOW_LINE, ETC};
AppState currentState = STOP;

bool debugMode = false;

void parseData(String& receivedData)
{
  bool isFromBLE = false;

  if (receivedData.length() >= 2)
  {
    // Vérifier si les deux premiers octets sont 0xFF55 (BLE) --> (Header)
    if ((uint8_t)receivedData[0] == 0xFF && (uint8_t)receivedData[1] == 0x55)
    {
      // Supprimer les deux premiers octets du header (0xFF55)
      receivedData.remove(0, 2);
      isFromBLE = true;
      handleCommand(receivedData);
    }
    // Vérifier si les deux premiers caractères sont "!!" (Moniteur Série)
    else if (receivedData.startsWith("!!"))
    {
      // Supprimer les deux premiers octets du header (!!)
      receivedData.remove(0, 2);
      handleCommand(receivedData);
    }
    else
    {
      // En-tête non reconnue
      Serial.print(F("Données non reconnues : "));
      Serial.println(receivedData);
      return;
    }
  }
  else
  {
    Serial.print(F("Données trop courtes : "));
    Serial.println(receivedData);
    return;
  }

  if (debugMode)
  {
    Serial.print(F("Reçu du "));
    Serial.print(isFromBLE ? F("BLE") : F("Moniteur Série"));
    Serial.print(" : ");
    Serial.println(receivedData);
  }
}

void handleCommand(const String& cmd)
{

  int index = cmd.indexOf(',');
  if (index == -1)
  {
    // Commande sans paramètre
    if (cmd == "FOLLOW_LINE")
    {
      currentState = FOLLOW_LINE;
    }
    else if (cmd == "STOP")
    {
      currentState = STOP;
    }
    else 
    {
      currentState = ETC;
    }
  }
  else
  {
    String leftPart = cmd.substring(0, index);
    String rightPart = cmd.substring(index + 1);

    if (leftPart == "VITESSE")
    {

    }
    else if (leftPart == "KP")
    {
      //kp = rightPart.toFloat();
    }
    else if (leftPart == "KI")
    {
      //ki = rightPart.toFloat();
    }
    else if (leftPart == "KD")
    {
      //kd = rightPart.toFloat();
    }
  }
}

// Événement qui se déclenche lorsqu'il y a réception de données via le port série
void serialEvent()
{
  static String receivedData = "";

  if (!Serial.available()) return;

  receivedData = Serial.readStringUntil('\n');
  parseData(receivedData);
}


void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:

  Serial.println(F("Setup complété!"));
}

void loop() {
  // put your main code here, to run repeatedly:

}
