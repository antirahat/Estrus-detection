#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  float accelX;
  float accelY;
  float accelZ;
  float gyroX;
  float gyroY;
  float gyroZ;
  float temperature;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);

  // Copy the received data into myData
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Received %d bytes of data\n", len);

  // Print the received data
  Serial.println("Received sensor data:");
  Serial.printf("Acceleration -> X: %.2f, Y: %.2f, Z: %.2f\n", myData.accelX, myData.accelY, myData.accelZ);
  Serial.printf("Gyroscope    -> X: %.2f, Y: %.2f, Z: %.2f\n", myData.gyroX, myData.gyroY, myData.gyroZ);
  Serial.printf("Temperature  -> %.2f °C\n", myData.temperature);
  Serial.println();
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register the callback function for receiving data
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  // Nothing to do in the loop as the ESP-NOW callback handles incoming data
  delay(1000);
}
