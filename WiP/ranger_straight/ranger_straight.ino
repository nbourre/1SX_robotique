#include <MeAuriga.h>

unsigned long currentTime = 0;

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeGyro gyro(0, 0x69);


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

void initEncoders() {
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);

  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
}

void setup()
{
  initEncoders();

  Serial.begin(115200);

  gyro.begin();
}

void loop() {
  currentTime = millis();
  // put your main code here, to run repeatedly:



  Encoder_1.loop();
  Encoder_2.loop();
  gyro.update();

  serialTask(currentTime);
}

void serialTask(unsigned long time){
  static unsigned long lastTime = 0;
  const int rate = 500;

  if (time - lastTime < rate) return;

  lastTime = time;

  Serial.print ("Batt : ");
  Serial.print (analogRead(A4));
  Serial.println("");
}
