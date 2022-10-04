/*
* Requirements : an analog device connected on A1
* Open Serial Plotter in tools to view the result
*/

int soundValue = 0;
int sound_avg = 0;
long sound_sum = 0;
int sound_count = 0;
int sample_size = 32;

void setup() {
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  soundValue = analogRead(A1);
  sound_sum += soundValue;
  if (sound_count == sample_size - 1){
    sound_avg = sound_sum >> 5; // right shift de 2^5 (comme divisé par 32)
    sound_count = 0;
    sound_sum = 0;
    Serial.print("max:1024,min:0,");
    Serial.print("avg:");
    Serial.print(sound_avg);
    Serial.println(",");
  }
  
  sound_count = (sound_count + 1) % sample_size;
}
