# -*- coding: utf-8 -*-
"""
Created on Fri Jan 15 15:31:40 2016

@author: Alvaro
"""

import timeit
import time
import serial
import struct
import numpy as np

ser = serial.Serial('/dev/ttyUSB0',115200,timeout=None)
#ser.setDTR(False)
#time.sleep(1)
#ser.flushInput()
#ser.setDTR(True)
#time.sleep(1)

header = chr(0xAA)
footer = chr(0xBB)
check = 0x00
data = struct.pack('f',float(100.0))

dataBack = ''

jarl = timeit.default_timer()
for i in range(len(data)):
    check = check^ord(data[i])
check = chr(check)
message = header+data+check+footer
ser.write(message)
print jorl-jarl
for i in range(len(data)):
    dataBack = dataBack+ser.read()
dataBack = round(struct.unpack('f',dataBack)[0],4)
