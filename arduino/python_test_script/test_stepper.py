import serial
import numpy as np
import time

freq = 100 # Hz

dt = 1e9/freq # in ns.
t = np.linspace(0, 100, freq*100+1)
series = 2560*np.sin(2*np.pi*t)
series = series.astype(int).astype(str)


with serial.Serial(port="/dev/ttyUSB0", baudrate=500000, timeout=60) as ser:
    time.sleep(6)
    time_old = time.perf_counter_ns()

    for i in range(len(t)):
        time_not_passed = True
        while time_not_passed:
            time_new = time.perf_counter_ns()
            time_not_passed = ((time_new - time_old) < dt)

        # print((time_new-time_old)/1_000_000_000)
        time_old = time_new
        msg = series[i] + '\n'
        ser.write(msg.encode())
        # print(i, series[i])
