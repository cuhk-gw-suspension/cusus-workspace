import serial
import numpy as np
import time

freq = 100 # Hz

dt = 1_000_000_000/freq # in ns.
t = np.linspace(0, 100)
series = 2*25600*np.sin(t)
series = series.astype(int).astype(str)


with serial.Serial(port="/dev/ttyUSB0", baudrate=500000, timeout=5) as ser:
    time_old = time.perf_counter_ns()

    for i in range(len(t)):
        time_passed = False
        while not time_passed:
            time_new = time.perf_counter_ns()
            time_passed = time_new - time_old < dt

        time_old = time_new
        msg = series[i] + '\n'
        ser.write(msg)
        print(i, series[i])
