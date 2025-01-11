#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <PubSubClient.h>



// Wi-Fi credentials
const char* ssid = "Rahat";
const char* password = "rahat12345";

String macAddress = WiFi.macAddress();

// MQTT Broker
const char* mqtt_server = "broker.emqx.io"; // Replace with your broker address
String topic;
//macAddress.replace(":", ""); // Remove colons for a cleaner topic



// Create instances
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_MPU6050 mpu;

void setupWiFi() {
  delay(10);
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected.");
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("macAddress")) { // adjust MAC address of the device
      Serial.println("connected.");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds.");
      delay(1000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setupWiFi();

  
  String macAddress = WiFi.macAddress(); // Retrieve MAC address
  macAddress.replace(":", ""); // Remove colons for a cleaner topic
  topic = "esp32/" + macAddress + "/mpu6050"; // Example: esp32/24A1D3FF8C12/mpu6050
  Serial.println("MQTT Topic: " + topic);
  

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 initialized.");

  // Configure MQTT
  client.setServer(mqtt_server, 1883);

  // Sensor setup
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  //delay(100); //prevent watchdog timer 
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  String payload = String("{\"accel\":{\"x\":") + a.acceleration.x +
                   ",\"y\":" + a.acceleration.y + 
                   ",\"z\":" + a.acceleration.z + 
                   "},\"gyro\":{\"x\":" + g.gyro.x +
                   ",\"y\":" + g.gyro.y + 
                   ",\"z\":" + g.gyro.z + "}}" + 
                   ", \"temp\":" + temp.temperature ;

  if (client.publish(topic.c_str(), payload.c_str())) {
    Serial.println("Message sent: " + payload);
  } else {
    Serial.println("Message failed.");
  }

  delay(1000); // Adjust data sending frequency - now 1s
}
