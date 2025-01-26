#include <WiFi.h>
#include <time.h>

// Wi-Fi credentials
const char* ssid = "I_Farmer_NBO";
const char* password = "iFarmer_2018";

// NTP Server
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 21600;         // Adjust for your timezone
const int daylightOffset_sec = 0; // Adjust for Daylight Saving Time

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");

  // Sync time from NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Time synchronized from NTP server!");

  // Wait until the time is set
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  // Print the synchronized time
  Serial.println(&timeinfo, "RTC time synced: %Y-%m-%d %H:%M:%S");

  // Disconnect from Wi-Fi
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("Wi-Fi disconnected. RTC is running.");

  // Test RTC by printing time periodically
  delay(2000); // Simulate delay before entering loop
}

void loop() {
  // Get and print the current time from the RTC
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println(&timeinfo, "Current RTC time: %Y-%m-%d %H:%M:%S");
  } else {
    Serial.println("Failed to get time from RTC.");
  }
  delay(5000); // Print every 5 seconds
}
