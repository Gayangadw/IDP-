#include <WiFi.h>
#include "time.h"
#include "sntp.h"

const char* ssid = "GAYANGA";
const char* password = "Gayangaa";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 19800; // GMT offset for IST (19800 seconds = 5 hours 30 minutes)
const int daylightOffset_sec = 0; // No daylight saving time observed in India

const char* time_zone = "IST-5:30"; // Indian Standard Time (IST) with GMT+5:30 offset

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void setup() {
  Serial.begin(115200);

  // Set notification call-back function
  sntp_set_time_sync_notification_cb(timeavailable);

  // Enable NTP server mode via DHCP
  sntp_servermode_dhcp(1);

  // Configure time with NTP servers
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  // Connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
}

void loop() {
  delay(5000);
  printLocalTime(); // It will take some time to sync time :)
}
