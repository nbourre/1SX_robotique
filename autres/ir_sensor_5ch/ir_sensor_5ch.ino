// Based code for testing WaveShare Tracker sensor
// Src : https://www.waveshare.com/wiki/Tracker_Sensor

// Pin definitions
// For MEGA
const int sensorPins[] = {A8, A9, A10, A11, A12};
// For Atmega328 based and keeping i2c port
// const int sensorPins[] = {A0, A1, A2, A3, A6};


const int numSensors = 5;

// Variables to store analog values
int sensorValues[numSensors];

// Timer variables
unsigned long previousMillis = 0;
const long interval = 250;  // interval in milliseconds

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize sensor pins as inputs (optional, as they are inputs by default)
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

void loop() {
  // Check if 250 ms have passed
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the current time to compare in the next iteration
    previousMillis = currentMillis;

    // Read the sensor values
    for (int i = 0; i < numSensors; i++) {
      sensorValues[i] = analogRead(sensorPins[i]);
    }

    // Send the values through the serial port
    for (int i = 0; i < numSensors; i++) {
      Serial.print("Sensor_");
      Serial.print(i);
      Serial.print(":");
      Serial.print(sensorValues[i]);
      if (i < numSensors - 1) {
        Serial.print(",");
      }
    }
    Serial.println();
  }
}
