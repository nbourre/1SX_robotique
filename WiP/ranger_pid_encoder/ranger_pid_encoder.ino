#include <MeAuriga.h>

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);

#pragma region ISR

int setPoint = 100;
float kp = 0.18;
float ki = 0.0;
float kd = 0.0;
bool debugMode = false;

void isr_process_encoder1(void)
{
  if(digitalRead(Encoder_1.getPortB()) == 0)
  {
    Encoder_1.pulsePosMinus();
  }
  else
  {
    Encoder_1.pulsePosPlus();;
  }
}

void isr_process_encoder2(void)
{
  if(digitalRead(Encoder_2.getPortB()) == 0)
  {
    Encoder_2.pulsePosMinus();
  }
  else
  {
    Encoder_2.pulsePosPlus();
  }
}

#pragma endregion

void encodersUpdate() {

  Encoder_1.loop();
  Encoder_2.loop();
}

void processSerialInput() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toLowerCase(); // Make case insensitive
    
    // Toggle debug mode with "d" command
    if (input == "d") {
      debugMode = !debugMode;
      Serial.print("Debug mode: ");
      Serial.println(debugMode ? "ON" : "OFF");
      if (debugMode) {
        Serial.print("Current PID values - Kp: ");
        Serial.print(kp, 3);
        Serial.print(", Ki: ");
        Serial.print(ki, 3);
        Serial.print(", Kd: ");
        Serial.println(kd, 3);
      }
      return;
    }
    
    // Parse parameter setting (format: kp=0.18, ki=0.1, kd=0.05)
    int equalSign = input.indexOf('=');
    if (equalSign > 0) {
      String param = input.substring(0, equalSign);
      param.trim();
      float value = input.substring(equalSign + 1).toFloat();
      
      bool updated = false;
      
      if (param == "kp") {
        kp = value;
        updated = true;
      } else if (param == "ki") {
        ki = value;
        updated = true;
      } else if (param == "kd") {
        kd = value;
        updated = true;
      }
      
      if (updated) {
        // Apply new PID values to encoder
        Encoder_1.setSpeedPid(kp, ki, kd);
        
        // Confirmation message
        Serial.print(param);
        Serial.print(" updated to ");
        Serial.println(value, 3);
        
        if (debugMode) {
          Serial.print("All PID values - Kp: ");
          Serial.print(kp, 3);
          Serial.print(", Ki: ");
          Serial.print(ki, 3);
          Serial.print(", Kd: ");
          Serial.println(kd, 3);
        }
      } else {
        Serial.println("Invalid parameter. Use: kp=value, ki=value, or kd=value");
      }
    } else {
      Serial.println("Invalid format. Use: kp=0.18, ki=0.1, kd=0.05, or 'd' for debug toggle");
    }
  }
}

void printSerialTask(unsigned long ct) {
  static String msg = "";
  static unsigned long lastTime = 0;
  const int rate = 50;

  if (ct - lastTime < rate) return;

  lastTime = ct;
  
  msg = "setpoint:";
  msg += setPoint;
  msg += "\te1_speed:";
  msg += Encoder_1.getCurrentSpeed();
  
  if (debugMode) {
    msg += "\tPID(";
    msg += kp;
    msg += ",";
    msg += ki;
    msg += ",";
    msg += kd;
    msg += ")";
  }
  

  if (msg != "") {
    Serial.print(msg);
    Serial.println("");
  }
}

void setup()
{
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);

  Serial.begin(115200);
  
  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  Encoder_1.setPulse(9);
  Encoder_2.setPulse(9);
  Encoder_1.setRatio(39.267);
  Encoder_2.setRatio(39.267);
  Encoder_1.setPosPid(1.8,0,1.2);
  Encoder_2.setPosPid(1.8,0,1.2);
  Encoder_1.setSpeedPid(kp, ki, kd);
  Encoder_2.setSpeedPid(0.18,0,0);

  Encoder_1.runSpeed(setPoint);
  
  Serial.println("PID Tuning Demo");
  Serial.println("Commands: kp=0.18, ki=0.1, kd=0.05, 'd' to toggle debug");
}

void loop() {
  unsigned long currentTime = millis();

  processSerialInput();
  encodersUpdate();

  printSerialTask(currentTime);
}
