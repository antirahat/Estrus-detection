// include libraries
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Time.h>
#include <vector>
#include <WiFiManager.h>


// Wi-Fi credentials
// const char* ssid = "Rahat";
// const char* password = "rahat12345";

// MQTT Broker
const char* mqtt_server = "broker.emqx.io"; 
String topic;

// Create instances
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_MPU6050 mpu;
ESP32Time rtc; // Create an ESP32Time instance
std::vector<String> dataQueue;

// Wi-Fi setup
void setupWiFi() {
  // Serial.println("Connecting to Wi-Fi...");
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("\nWi-Fi connected.");
  
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);

  WiFiManager wm;

  bool res;

  res = wm.autoConnect("cowdy_ifarmer");

  if(!res) {
    Serial.println("Failed to connect");

  }
  else {
    Serial.println("Connected to wifi");
  }
  // Set the initial time (NTP or manual configuration could be added here)
  // This is a placeholder for testing; adjust as necessary
  //rtc.setTime(0); // Epoch timestamp; set to 0 for 1970-01-01 00:00:00
}

// Get current timestamp using ESP32Time
unsigned long getTimestamp() {
  return rtc.getEpoch(); // Returns the Unix timestamp
}

// MQTT connection
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("macAddress")) {
      Serial.println("connected.");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds.");
      delay(1000);
    }
  }
}

// Publish queued data
void sendQueuedData() {
  if (!dataQueue.empty()) {
    Serial.println("Sending queued data...");
    for (const String& payload : dataQueue) {
      if (client.publish(topic.c_str(), payload.c_str())) {
        Serial.println("Sent from queue: " + payload);
      } else {
        Serial.println("Failed to send from queue: " + payload);
        break; // Stop if publishing fails
      }
    }
    dataQueue.clear(); // Clear the queue after sending
  }
}

void setup() {
  Serial.begin(115200);

  

  setupWiFi();

  String macAddress = WiFi.macAddress();
  macAddress.replace(":", "");
  topic = "esp32/" + macAddress + "/mpu6050";
  Serial.println(topic);
  Serial.println("MQTT Topic: " + topic);

  // if (!setCpuFrequencyMhz(240)) {
  //   Serial.println("Failed to set CPU frequency to 80 MHz");
  //   while (1) {
  //     delay(10);
  //   }
  // }
  // Serial.println("CPU frequency set to 80 MHz");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 initialized.");

  client.setServer(mqtt_server, 1883);

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Storing data locally...");
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Construct JSON payload with timestamp
    String payload = "{";
    payload += "\"timestamp\":" + String(getTimestamp()) + ",";
    payload += "\"accel\":{";
    payload += "\"x\":" + String(a.acceleration.x, 2) + ",";
    payload += "\"y\":" + String(a.acceleration.y, 2) + ",";
    payload += "\"z\":" + String(a.acceleration.z, 2);
    payload += "},";
    payload += "\"gyro\":{";
    payload += "\"x\":" + String(g.gyro.x, 2) + ",";
    payload += "\"y\":" + String(g.gyro.y, 2) + ",";
    payload += "\"z\":" + String(g.gyro.z, 2);
    payload += "},";
    payload += "\"temp\":" + String(temp.temperature, 2);
    payload += "}";

    dataQueue.push_back(payload); // Store data in local queue
    delay(1000); // Adjust data collection frequency
    return;
  }

  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Send any queued data if connected
  sendQueuedData();

  // Read new data from the sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  String payload = "{";
  payload += "\"timestamp\":" + String(getTimestamp()) + ",";
  payload += "\"accel\":{";
  payload += "\"x\":" + String(a.acceleration.x, 2) + ",";
  payload += "\"y\":" + String(a.acceleration.y, 2) + ",";
  payload += "\"z\":" + String(a.acceleration.z, 2);
  payload += "},";
  payload += "\"gyro\":{";
  payload += "\"x\":" + String(g.gyro.x, 2) + ",";
  payload += "\"y\":" + String(g.gyro.y, 2) + ",";
  payload += "\"z\":" + String(g.gyro.z, 2);
  payload += "},";
  payload += "\"temp\":" + String(temp.temperature, 2);
  payload += "}";

  if (client.publish(topic.c_str(), payload.c_str())) {
    Serial.println("Message sent: " + payload);
  } else {
    Serial.println("Message failed.");
  }

  delay(1000); // Adjust data sending frequency
}
