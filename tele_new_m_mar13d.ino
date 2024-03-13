#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HX711_ADC.h>
#include <WiFiClientSecure.h>  
#include <UniversalTelegramBot.h>


const int HX711_dout = 16; 
const int HX711_sck = 4;   


HX711_ADC LoadCell(HX711_dout, HX711_sck);


const char* ssid     = "########";
const char* password = "############";


#define BOT_TOKEN "##########################################"  // Replace with your bot token
#define CHAT_ID "#######"      

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);


int lcdColumns = 16;
int lcdRows = 2;


LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

String messageStatic = "Weight: ";

void setup() {
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  
  Serial.begin(115200); delay(10);
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

  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
}

void loop() {
  static boolean newDataReady = false;

 
  if (LoadCell.update()) {
    newDataReady = true;
  }

  
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    if (text == "/weight") {
      float currentWeight = LoadCell.getData();
      String response = "Current Weight: " + String(currentWeight);
      bot.sendMessage(chat_id, response, "");
    }
  }

  
  if (newDataReady) {
    float weight = LoadCell.getData();
    lcd.setCursor(0, 0);
    lcd.print(messageStatic);
    lcd.print(weight);
    newDataReady = false;
  }

  
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}
