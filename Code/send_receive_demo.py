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

ser = serial.Serial('/dev/ttyUSB0',1000000,timeout=None)
ser.setDTR(False)
time.sleep(1)
ser.flushInput()
ser.setDTR(True)
time.sleep(5)

ok = 0;

header = chr(0xAA)
footer = chr(0xBB)
check = 0x00
data = struct.pack('f',float(100.0))

tic = timeit.default_timer()
for i in range(len(data)):
    check = check^ord(data[i])
check = chr(check)
message = header+data+check+footer
ser.write(message)
toc = timeit.default_timer()
print toc-tic

time.sleep(1)

xorChk = 0x00
rec = chr(0x00)
dataSize = 4
dataBack = ''
i = 0

tic = timeit.default_timer()
while(ser.read() != header): None

while(ser.inWaiting()>0):
    rec = ser.read()
    if((rec!=footer) and (i<dataSize)):
        dataBack = dataBack + rec
        i += 1
        if(i == 4):
            checkBack = ser.read()
    if(rec == footer):
        i = 0
        break

for i in range(len(dataBack)):
    xorChk = xorChk^ord(dataBack[i])
xorChk = chr(xorChk)

if(xorChk == checkBack):
    dataBack = round(struct.unpack('f',dataBack)[0],4)
    print 'Oh yeah!'
else:
    print 'Oh no!'
toc = timeit.default_timer()
print toc-tic
