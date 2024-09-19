# This class will SEND input to the arduino

# Make sure to check the README to download requirements, use a venv (optional)

import serial
import time

#maybe set baudrate to 115200?
# Change port to match OS, windows is COM4, linux is /dev/ttyUSB0
arduino = serial.Serial(port='/dev/ttyUSB0',   baudrate=9600, timeout=.1)

def write_read(x):
    arduino.write(bytes(x,   'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return   data

while True:
    num = input("Enter a number: ")
    value   = write_read(num)
    print(value)