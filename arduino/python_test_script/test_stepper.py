import serial
import numpy as np
import time
import matplotlib.pyplot as plt
from scipy.signal import welch

fs = 100 # Hz

dt = 1e9/fs # in ns.
period = 300
t = np.linspace(0, period, num=fs*period+1)
freq = np.linspace(0, 20, num=fs*period+1) + 0.1
# freq=1
series = 1280*np.sin(2*np.pi*freq*t)

# series = np.random.normal(0, 100, len(t))

# f, psd0 = welch(series, fs=fs, nperseg=int(len(t)/5t)
# f, psd1 = welch(series.astype(int), fs=fs, nperseg=int(len(t)/5))
# plt.loglog(f, psd0)
# plt.loglog(f, psd1)
# plt.show()

series = series.astype(int).astype(str)
with serial.Serial(port="/dev/ttyUSB0", baudrate=500000, timeout=10) as ser:
    line = ser.readline().decode()
    print(line)
    # ser.write("R\n".encode())

    time_old = time.perf_counter_ns()

    for i in range(len(t)):
        time_not_passed = True
        while time_not_passed:
            time_new = time.perf_counter_ns()
            time_not_passed = ((time_new - time_old) < dt)

        # print((time_new-time_old)/1_000_000_000)
        time_old = time_new
        msg = 'M' + series[i] + '\n'
        ser.write(msg.encode())
        print(i, series[i])
        # line = ser.readline().decode()
        # print(line, end="")

