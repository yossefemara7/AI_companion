import serial
import time

# Set up the serial connection (adjust '/dev/ttyUSB0' as needed)
ser = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(2)  # Wait for the connection to establish

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()  # Read a line from Arduino
        print(line)  # Print the received data