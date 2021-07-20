import time
from concurrent.futures import ThreadPoolExecutor, as_completed
import numpy as np
import serial

from serialmon.utils.log import log_serial_info


args = (10,"~/sweep_sine/trail.csv")
kwargs = {"port_path":"/dev/ttyUSB1", "baud":19200, "timeout":10}

def sweepSin(time, freq):
    dt = 1e9/freq # in ns.
    t = np.linspace(0, time, num=freq*10+1)
    series = 25600*np.sin(2*np.pi/5*t)
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
            print(i, series[i], end=" ")
            line = ser.readline().decode()
            print(line, end="")
    return "sweep finished"

with ThreadPoolExecutor(max_workers=2) as executor:
    f1 = executor.submit(log_serial_info, *args, **kwargs)
    f2 = executor.submit(sweepSin, 5, 100)

    for f in as_completed([f1, f2]):
        print(f.result())



