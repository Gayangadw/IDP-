#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// HX711 pins:
const int HX711_dout = 16; 
const int HX711_sck = 4;   


HX711_ADC LoadCell(HX711_dout, HX711_sck);

//network credentials
const char* ssid     = "####";
const char* password = "######";


int lcdColumns = 16;
int lcdRows = 2;


LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

String messageStatic = "Weight: ";

void setup() {
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  
#if defined(ESP8266)|| defined(ESP32)
 
#endif

  unsigned long stabilizingtime = 2000;
  boolean _tare = true;
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU > HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(316.18);
    Serial.println("Startup is complete");
  }
}

void loop() {
  static boolean newDataReady = 0;

 
  if (LoadCell.update()) newDataReady = true;

  if (newDataReady) {
    float weight = LoadCell.getData();
    lcd.setCursor(0, 0);
    lcd.print(messageStatic);
    lcd.print(weight);
    newDataReady = 0;
  }

  
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}
