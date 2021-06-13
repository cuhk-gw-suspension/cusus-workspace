import subprocess
import os
from .adaptOS import get_nano_port

def upload(file_path, port=get_nano_port(), timeout=60):
    '''function to upload .ino script to chip within python

    Parameters:
    ------------
    file_path: string
            absolute path or relative path of the script to be uploaded.
    port: string, optional
            usb port to connect to the arduino chip, default to nano's port.
    timeout: int, optional
            timeout in seconds, default is 60.
    '''

    ret = subprocess.run(["arduino", "--upload", "--port", port, file_path],
                         shell=False,
                         encoding="utf-8",
                         timeout=timeout)

    if ret.returncode == 0:
        print("success:",ret)
    else:
        raise Exception("error:",ret)

if __name__ == "__main__":
    fpath = os.path.join("nano_script", "ReadAccelerometer",
            "ReadAccelerometer.ino")
    upload(fpath)
    pass
