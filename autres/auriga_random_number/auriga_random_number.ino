/**
 * \par Copyright (C), 2023, Nick B
 * @file    auriga_random_number.ino
 * @author  Nick B
 * @version V1.0.0
 * @date    2023/11/27
 * @brief   This file shows the usage of random numbers
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Nick B     2023/11/27    1.0.0          Raondom leds
 * </pre>
 */

#include <MeAuriga.h>

#define DEBUG 1
#define LEDNUM  12

// on-board LED ring, at PORT0 (onboard)
MeRGBLed led( 0, LEDNUM );

unsigned long currentTime = 0;

// Lights up a segment of the ring
// Dependency: None
void ledShowSegment(short startIdx, short nbLeds, uint8_t red, uint8_t green, uint8_t blue) {
  int currentIdx = startIdx;
  
  for (int i = 0; i < nbLeds; i++) {
    currentIdx = (startIdx + i) % LEDNUM;
    led.setColorAt(currentIdx, red, green, blue);
    
  }
  led.show();
}

// Function to return a value
// indicating the decision to be made for
// the robot project
// Return: 0 -> Right, 1 -> Left
// Dependency: randomLedTask, ledOff, ledShowSegment
int getDirection() {
  int result = 0;
  unsigned long seed = millis() + analogRead(A2) + analogRead(A3);
  randomSeed(seed);
  
  unsigned long endTime = millis() + 3000;
  result = random(100);
  
#if DEBUG
  Serial.print("Seed : ");
  Serial.print(seed);
  Serial.print("\tRandom : ");
  Serial.println(result);
#endif

  result = result < 50 ? 0 : 1;
  
  while (millis() < endTime) {
    randomLedTask(millis());
  }
  
  ledOff();

  ledShowSegment(result ? 4 : 10, 3, 0, 50, 0);
  
  endTime = millis() + 1000;
      
  while (millis() < endTime) {
    // To give time to see the green LEDs
    // I wait 1000 ms
  }
  
  ledOff(); 
     
  return result;
}

// Lights up random LEDs
// Return: index of the LED
// Dependency: None
int randomLedTask (unsigned long time) {
  static unsigned long  lastTime = 0;
  static unsigned int lastIdx = 0;
  static int i  = 0;
  static int r, g, b = 0;
  
  const int rate = 50;
  
  if (time - lastTime < rate) return;
  
  lastTime = time;
  
  i = random(0, 12);
  r = random(0, 127);
  g = random(0, 127);
  b = random(0, 127);
  
  led.setColorAt(i, r, g, b);
  led.setColorAt(lastIdx, 0, 0, 0);
  led.show();
  
  lastIdx = i;
  return i;
}

// Turns off all the LEDs
void ledOff(){
  led.setColor(0, 0, 0);
  led.show();
}

void initRing() {
  led.setpin( 44 );
}

void setup() {
  Serial.begin(115200);
  
  initRing();
  
  // For demonstration only
  // stays in this infinite loop
  while (true) {
    int result = getDirection();
    Serial.print("Final result : ");
    Serial.println(result);
    delay(50);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
