#include <MeAuriga.h>

MeLineFollower lineFollower(PORT_9);

void setup() {
    Serial.begin(115200);
}

void loop() {
    int sensors = lineFollower.readSensors();
    Serial.print("Valeur:");
    Serial.print(sensors);
    Serial.print("\tDesc : ");
    
    switch (sensors) {
      case 0:
        Serial.println("Les deux capteurs voit la ligne noire.");
        break;      
      case 1:
        Serial.println("S1 voit la ligne et S2 ne voit pas la ligne");
        break;      
      case 2:
        Serial.println("S1 ne voit pas la ligne et S2 voit la ligne");
        break;      
      case 3:
        Serial.println("Les deux capteurs ne voit pas la ligne");
        break;
    }
    
    delay(100);
}