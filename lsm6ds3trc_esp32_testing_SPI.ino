#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

#define LSM_CS   5   // Chip Select pin
#define LSM_SCK  18  // SPI Clock
#define LSM_MISO 19  // Master In Slave Out
#define LSM_MOSI 23  // Master Out Slave In

Adafruit_LSM6DS3TRC lsm6ds3trc;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // Wait for Serial to initialize

  Serial.println("Adafruit LSM6DS3TR-C test!");

  // Initialize SPI communication
  if (!lsm6ds3trc.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
    Serial.println("Failed to find LSM6DS3TR-C chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("LSM6DS3TR-C Found!");
}

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;

  lsm6ds3trc.getEvent(&accel, &gyro, &temp);

  Serial.print("Accel X: "); Serial.print(accel.acceleration.x); Serial.print(" m/s^2, ");
  Serial.print("Y: "); Serial.print(accel.acceleration.y); Serial.print(" m/s^2, ");
  Serial.print("Z: "); Serial.print(accel.acceleration.z); Serial.println(" m/s^2");

  Serial.print("Gyro X: "); Serial.print(gyro.gyro.x); Serial.print(" rad/s, ");
  Serial.print("Y: "); Serial.print(gyro.gyro.y); Serial.print(" rad/s, ");
  Serial.print("Z: "); Serial.print(gyro.gyro.z); Serial.println(" rad/s");

  delay(500);
}
