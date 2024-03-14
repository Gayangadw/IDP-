#ifdef ESP32
  #include <WiFi.h>
  #include <ESP32Servo.h> 
#else
  #include <ESP32WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

#define BOTtoken "6866460866:AAE_-QwQOXLMQngdnFiLWK4fbY3RsHnmIOY"  
#define CHAT_ID "5157665426" 

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif


const char* ssid = "GAYANGA";
const char* password = "Gayangaa";

String messageStatic = "SMART PET FEEDER";


int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


int botRequestDelay = 1000;
unsigned long lastTimeBotRan;


Servo myservo;


const int servoPin = 13;


bool feeding = false;


void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void setup() {
  Serial.begin(115200);

 
  lcd.init();
                       
  lcd.backlight();
  
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      
    client.setTrustAnchors(&cert); 
  #endif

 
  myservo.attach(servoPin);

 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());

  /
  lcd.setCursor(0, 0);
  lcd.print(messageStatic);
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}


void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
   
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/feed" && !feeding) {
      feeding = true;
      lcd.setCursor(0, 1);
      lcd.print("Feeding starting");
      bot.sendMessage(chat_id, "Feeding starting", "");
      
      for (int j = 0; j < 3; j++) {
        
        myservo.write(135);
        delay(1000); 
        
       
        myservo.write(0);
        delay(1000);
      }
      
      lcd.setCursor(0, 1);
      lcd.print("Feeding complete ");
      bot.sendMessage(chat_id, "Feeding complete", "");
      
      feeding = false;
    }
  }
}
