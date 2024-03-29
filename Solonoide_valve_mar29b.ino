#define RELAY_PIN 16 
void setup() {
  pinMode(RELAY_PIN, OUTPUT);
}


void loop() {
  digitalWrite(RELAY_PIN, HIGH); //valve open for 5 seconds
  delay(5000);
  digitalWrite(RELAY_PIN, LOW);  //valve open for 5 seconds
  delay(5000);
}