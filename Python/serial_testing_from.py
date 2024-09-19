# This class will recieve input FROM the arduino

# Make sure to check the README to download requirements, use a venv (optional)

import serial
import time

# Set up the serial connection
# # Change port to match OS, windows is COM4, linux is /dev/ttyUSB0
ser = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(2)  # Wait for the connection to establish

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()  # Read a line from Arduino
        print(line)  # Print the received data