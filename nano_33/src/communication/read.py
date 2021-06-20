import serial
import time
import os
import traceback
from .adaptUSBport import get_nano_port

def export_measurements(
        elapsed_time, 
        dir_path,
        filename,
        port_path=get_nano_port(), 
        baud=2_000_000, 
        timeout=1):
    '''
    Read serial data for an given time from Arduino and export to a tsv.

    Parameters
    ------------
    elapsed_time: float or int
                elapsed time for reading the serial print, in seconds.
    dir_path: string
                path to directory storing serial readings.
    port_path: string, optional
                path to the Arduino port, default to nano 33 ble port path.
    baud: int, optional
                baud rate of the serial communication.
    timeout: int, optional
                timeout for reading the serial print, in seconds.

    '''
    if not os.path.exists(port_path):
        raise IOError("Could not find the specified Arduino port")

    if not os.path.exists(dir_path):
        os.makedirs(dir_path)

    file_path = os.path.join(dir_path, filename)
    try:
        mycsv = open(file_path, 'w')
    except IOError:
        print("failed to open file")
        traceback.print_exc()
    except:
        print(repr(Exception))
        traceback.print_exc()

    if timeout is None:
        raise Exception('timeout must be specified.')

    print('starting trial, ETA: %.2f seconds'%elapsed_time)

    start = time.perf_counter()
    with serial.Serial(port_path, baud, timeout=timeout) as ser:
        while (time.perf_counter() - start < elapsed_time):
            line = ser.readline()
            mycsv.write(line.decode('UTF-8'))
    mycsv.close()
    print("data logged successfully.")

def monitor(port_path=get_nano_port(), baud=2_000_000, timeout=1):
    '''
    Read serial data indefinitely from Arduino device.

    Parameters
    ------------
    port_path: string, optional
                path to the Arduino port, default to nano 33 ble.
    baud: int, optional
                baud rate of the serial communication.
    timeout: int, optional
                timeout for reading the serial print, in seconds.

    '''
    with serial.Serial(port_path, baud, timeout=timeout) as ser:
        while True:
            line = ser.readline()
            print(line.decode('UTF-8'), end='')

        if timeout is None:
            raise Exception('timeout must be specified.')


if __name__=="__main__":
    export_measurements(60*60, "measurements", "119Hz_2g.csv")
    # monitor("/dev/ttyS4")
    pass
