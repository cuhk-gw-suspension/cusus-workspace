import serial
from src.communication.adaptUSBport import get_nano_port
from src.communication.upload_script import upload
import time
import os
import numpy as np

# script_path = os.path.join("nano_script", "ReadAccelerometer", "ReadAccelerometer.ino")
script_path = os.path.join("nano_script", "speedtest", "speedtest.ino")
upload(script_path)

start = time.perf_counter()
with serial.Serial(get_nano_port(), 2000000, timeout=1) as ser:
    i = 0
    temp = 0
    old = time.perf_counter_ns()
    while (time.perf_counter() - start < 2):
        line = ser.readline()
        new = time.perf_counter_ns()
        temp += new - old
        print(new - old, line.decode('UTF-8'))
        old = new
        i += 1
print("-------------------")
print("average receive interval: ", temp/i, "ns")
print("average Hz:", i/(temp*1e-9), "Hz")
