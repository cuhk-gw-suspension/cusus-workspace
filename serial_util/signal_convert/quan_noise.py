#!/usr/bin/env python
# coding: utf-8
import numpy as np


def quantize(analog_signal, reference_point, LSB):
    '''
    Convert analog signal to digital signal.
    
    Parameters:
    -------------
    analog_signal: ndarray
                input analog signal
    reference_point: float
                value at which analog signal and digital signal equals exactly.
    LSB: float
                Least significant bit(LSB) or the step size when converting to digital.
    
    Returns：
    -------------
    quan_signal: ndarray
                digital signal converted.
    quan_noise: ndarray
                error between digital and analog signal.
    '''
    normalized_signal = (analog_signal - reference_point)/LSB
    quan_signal = np.round(normalized_signal)    
    quan_signal *= LSB
    quan_signal += reference_point
    quan_noise = quan_signal - analog_signal
    return (quan_signal, quan_noise)
