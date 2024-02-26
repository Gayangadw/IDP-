#define RELAY_PIN 2 
#define WATER_LEVEL_SENSOR_PIN 3 

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); 
  pinMode(WATER_LEVEL_SENSOR_PIN, INPUT);
}

void loop() {
  int waterLevel = digitalRead(WATER_LEVEL_SENSOR_PIN);
  
  if (waterLevel == LOW) {
    
    digitalWrite(RELAY_PIN, HIGH);
  } else {
   
    digitalWrite(RELAY_PIN, LOW);
  }
  
  delay(1000); 
}


