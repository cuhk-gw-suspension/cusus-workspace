
class ADS1115:
"""A wrapper around LabjackPython U3's I2c interface for ADS1115 ADC devices.
"""
    def __init__(self, u3_device, address, sda_pin, scl_pin):
        """Constructor

        Parameters
        ----------
        u3_device : U3 object
            a U3 device object from LabjackPython.
        address : int
            I2C address.
        sda_pin : int
            The FIO / EIO pin used as SDA pin.
        scl_pin : int
            The FIO / EIO pin used as SCL pin.
        """
        self._device = u3_device
        self.address = address
        self._sda_pin = sda_pin
        self._scl_pin = scl_pin
        self._REGISTER_ADDR = {"Conversion": 0x00,
                               "Config": 0x01,
                               "Lo_thresh": 0x02,
                               "Hi_thresh": 0x03}

    def config(self, gain=4.096, ain_config=4, mode="single"):
        """Configures the ADS1115.

        Parameters
        ----------
        gain : float
            Gain amplifier configuration.
            Gain from [6.144, 4.096, 2.048, 1.024, 0.512, 0.256] can be used.
            A ValueError would be raised if other values was used.
        ain_config : int
            input multiplexer configuration for ads1115.
            ain_config range from 0 to 7, corresponding to the respective bits
            for input selection.
        mode : str, optional
            Set the operation mode of ADS1115.
            mode from ['single', 'continuous'] can be selected.
        rate : int, optional
            SPS of ads1115.
            rate from [8, 16, 32, 64, 128, 250, 475, 860] can be selected.
            Default to 128SPS.
        """
        if not isinstance(ain_config, int):
            raise ValueError("ain_config must be integer."
        if ain_config > 7:
            raise ValueError("ain_config must be within range(8).")
        gain_options = [6.144, 4.096, 2.048, 1.024, 0.512, 0.256]
        mode_options = ["continuous", "single"]
        rate_options = [8, 16, 32, 64, 128, 250, 475, 860]
        1stbyte = mode_options.index(mode)

        1stbyte <<= 2
        1stbyte += ain_config

        1stbyte <<= 2
        1stbyte += gain_options.index(gain)

        1stbyte <<= 1
        1stbyte += mode_options.index(mode)

        2ndbyte = rate_options.index(rate)

        2ndbyte <<= 1
        2ndbyte += comp_mode_options.index(comp)

        sent_bytes = [1stbyte, 2ndbyte]


    def read(self, debug=False):
        """Read bits from ads1115. 
        If ads1115.config was never called, default mode will be used. 
        (see specsheet)

        Parameters
        ----------
        debug : bool, optional
            If true, set u3 to debug mode. Default to False.
        """
        self._device.debug = debug
        res = self._device.i2c(self.addres,
                               self._REGISTER_ADDR["Conversion"],
                               SDAPinNum=self._sda_pin,
                               SCLPinNum=self._scl_pin,
                               NumI2CBytesToReceive=2)
        returned = bytes(res["I2CBytes"])
        val = int.from_bytes(returned, byteorder="big", signed=True)



