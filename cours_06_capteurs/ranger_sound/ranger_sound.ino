int soundValue = 0;
int sound_avg = 0;
long sound_sum = 0;
int sound_count = 0;
int sample_size = 64;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  soundValue = analogRead(A1);
  sound_sum += soundValue;
  if (sound_count == sample_size - 1){
    sound_avg = sound_sum >> 6; // right shift de 2^5 (comme divisé par 32)
    sound_count = 0;
    sound_sum = 0;
    Serial.print("avg:");
    Serial.print(sound_avg);
    Serial.println(";");
  }
  
  sound_count = (sound_count + 1) % sample_size;
}
