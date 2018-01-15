import serial
import sys
import time

import pdb; pdb.set_trace()
print("HELLO FROM THE OTHER WORLD")
ser = serial.Serial("COM4", baudrate = 9600, timeout = 2)

time.sleep(1)


print("HELLO FROM THE OTHER WORLD")


