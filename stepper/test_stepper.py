import serial
import numpy as np

t = np.linspace(0, 100, num=500000)
series = 2*25600*np.sin(t)
series = series.astype(int).astype(str)

with serial.Serial(port="/dev/ttyUSB0", baud=500000, timeout=5) as ser:
    for i in range(len(t)):
        msg = series[i] + '\n'
        ser.write(msg)
        print(i, series[i])
