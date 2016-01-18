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
#data = chr(0x10)+chr(0x11)+chr(0x12)+chr(0x13)
data = struct.pack('f',float(100.0))

dataBack = ''

jarl = timeit.default_timer()
for i in range(len(data)):
    check = check^ord(data[i])
check = chr(check)
message = header+data+check+footer
ser.write(message)
jorl = timeit.default_timer()
print jorl-jarl
for i in range(len(data)):
    jarl = timeit.default_timer()
    dataBack = dataBack+ser.read()
    jorl = timeit.default_timer()
    print jorl-jarl
dataBack = round(struct.unpack('f',dataBack)[0],4)
#ojete = ser.read()
#ser.close()



#iterations = 5000
#elapsed = np.zeros((iterations,1))
#
#jarl = timeit.default_timer()
#byte = struct.pack('f',float(9.8765))
#jorl = timeit.default_timer()
#print jorl-jarl
#
#jarl = timeit.default_timer()
#floating = struct.unpack('f',byte)[0]
#jorl = timeit.default_timer()
#print jorl-jarl
#
#for i in range(iterations):
#    jarl = timeit.default_timer()
#    ser.write(chr(0xAA))
#    jorl = timeit.default_timer()
#    elapsed[i] = jorl-jarl
#print np.mean(elapsed)
#ser.write(byte)
#ser.write(hex(0xBB))

#for b in byte:
#    ser.write(b)