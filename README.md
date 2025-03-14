# Estrus Detection System

Through Cowdy, we aim to detect precise estrus periods, potential diseases, and abnormalities among cattle to ensure high production yield in livestock farms. The system uses MPU6050 sensors with ESP32 microcontrollers to collect and analyze movement data for accurate estrus detection.

## Overview

This project implements a sophisticated estrus detection system using motion sensors and machine learning. The system continuously monitors cow behavior patterns through accelerometer and gyroscope data to identify estrus periods with high accuracy.

### Key Features
- Real-time motion data collection using MPU6050 sensors
- Wireless data transmission via MQTT and ESP-NOW protocols
- Machine learning-based behavior classification
- MongoDB integration for data storage
- Automated estrus period detection

## Hardware Requirements

- ESP32 microcontroller
- MPU6050 accelerometer and gyroscope sensor
- Power supply unit
- Mounting equipment for sensor attachment

## Software Dependencies

- Arduino IDE with ESP32 board support
- Python 3.x
- Required Python packages:
  - scikit-learn
  - pandas
  - numpy
  - pymongo

## Installation

1. Clone this repository
2. Upload the appropriate firmware to ESP32:
   - Use `emqx_sending.ino` for MQTT implementation
   - Use `sender_espNOW.ino`/`receiver_espNOW.ino` for ESP-NOW implementation
3. Install Python dependencies:
   ```bash
   pip install scikit-learn pandas numpy pymongo
   ```

## How It Works

### Behavioral Pattern Classification
![image](https://github.com/user-attachments/assets/efcfbe5c-7f07-45d4-86e1-e4ede8fe1a5e)

The system classifies three main behavioral patterns from the sensor data:
- Feeding
- Rumination
- Resting

![image](https://github.com/user-attachments/assets/87fc2383-a363-4b29-a13b-998c1be771c9)

### Data Collection
Data is collected using iFarmer devices:
![image](https://github.com/user-attachments/assets/ac4eea39-9dd3-4696-a2af-18fa30f61288)

24-hour monitoring provides comprehensive behavioral data:
![image](https://github.com/user-attachments/assets/66da26b9-7c79-4b17-98b3-e9f3aedfd021)

The system has demonstrated successful estrus detection over 6 months (August-December 2019), with only one missed detection due to a power outage.

## Data Processing

The project includes several data processing scripts:
- `dataset_labeling.py`: For labeling collected sensor data
- `dt.py`: Decision tree implementation for classification
- `rfe.py`: Feature selection and engineering

Data is collected at different sampling rates (1s, 3s, 5s) and processed accordingly, as seen in the dataset files:
- filtered_gyro1s.csv
- filtered_gyro3s.csv
- filtered_gyro5s.csv

## Communication Protocols

The system supports two communication methods:
1. MQTT Protocol (`emqx_sending.ino`, `mqtt_to_mongoDB.ino`)
2. ESP-NOW Protocol (`sender_espNOW.ino`, `receiver_espNOW.ino`)

## Resources
1. [Research Paper on Estrus Detection](https://pmc.ncbi.nlm.nih.gov/articles/PMC7302546/)
2. [Algorithm Development Reference](https://www.mdpi.com/2624-7402/6/3/128)
3. [Labeled Dataset](https://figshare.com/articles/dataset/Accelerometer_and_Gryscope_Sensor_Data_from_Cows/25920463)

## Contact

For any additional queries, please contact:
Email: rahatalmamun20@gmail.com

## License

This project is licensed under the terms of the LICENSE file included in the repository.
