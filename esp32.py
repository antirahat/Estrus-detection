import serial
import csv
import time

# Set the serial port and baud rate
ser = serial.Serial('COM13', 115200)  # Replace 'COM13' with your ESP32's port

# Set the duration (in seconds) for data collection
duration = 20  # Collect data for 20 seconds

# Get the start time
start_time = time.time()

# Open a CSV file to save data
with open('imu_data.csv', 'w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow(["time", "accelX", "accelY", "accelZ", "gyroX", "gyroY", "gyroZ"])  # Column headers

    # Continuously read data from the serial port until 20 seconds have passed
    try:
        while time.time() - start_time < duration:
            # Read a line from the serial port
            line = ser.readline().decode('utf-8').strip()
            
            # Split the line into values
            data = line.split(',')
            
            # Write the data to the CSV file
            csvwriter.writerow(data)
            print("Data saved:", data)  # Optional: print data to monitor

    except KeyboardInterrupt:
        print("Data collection stopped manually.")

# Close the serial port
ser.close()
print("Data collection completed.")
