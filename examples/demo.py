# -*- coding: utf-8 -*-
"""
Created on Fri Jan 15 15:31:40 2016

@author: Alvaro
"""

import sys
import timeit
import time
import serial
import struct

header = chr(0xAA)
footer = chr(0xBB)

def connectToSerial():
    portname_start = ["/dev/ttyUSB","/dev/ttyACM","COM"]
    for port in portname_start:
        for i in range(10):
            portnum = str(i)
            portname_full = ''.join([port,portnum])
            try:
                ser = serial.Serial(portname_full, 115200, timeout=1)
                break
            except:
                if(i == 9 and port == "COM"):
                    print("No serial port found")
                    sys.exit(0)
    ser.setDTR(False)
    time.sleep(1)
    ser.flushInput()
    ser.setDTR(True)
    time.sleep(5)
    return ser

def checkMessage(message, check):
    xorCheck = 0x00
    for i in range(len(message)):
        for j in range(len(message[i])):
            xorCheck = xorCheck^ord(message[i][j])
    xorCheck = chr(xorCheck)
    if(xorCheck==check):
        return True
    else:
        return False

def getMessage(dataPoints):
    message = ''
    messageSize = dataPoints*4
    inByte = chr(0x00)
    i = 0
    while(ser.read()!=header): None
    while(ser.inWaiting()>0):
        inByte = ser.read()
        if(inByte!=footer and i<messageSize):
            message += inByte
            i += 1
        elif(inByte!=footer and i==messageSize):
            check = inByte
        elif(inByte==footer):
            return message, check

def receiveData():
    message, check = getMessage(1)
    if(checkMessage(message,check)):
        data = round(struct.unpack('f',message)[0],4)
        return data
    else:
        return None

def receiveList(dataPoints):
    message, check = getMessage(dataPoints)
    dataList = []
    i = 0
    if(checkMessage(message,check)):
        for j in range(dataPoints):
            data = ''
            for k in range(4):
                data += message[i]
                i += 1
            data = round(struct.unpack('f',data)[0],4)
            dataList.append(data)
        return dataList
    else:
        return None

def transmitData(dataToSend):
    check = 0x00
    data = struct.pack('f',float(dataToSend))
    for i in range(len(data)):
        check = check^ord(data[i])
    check = chr(check)
    message = header+data+check+footer
    ser.write(message)

def transmitList(listToSend):
    data = ''
    check = 0x00
    message = header
    for i in range(len(listToSend)):
        data = struct.pack('f',float(listToSend[i]))
        message += data
        for j in range(len(data)):
            check = check^ord(data[j])
    check = chr(check)
    message += check + footer
    ser.write(message)

dataPoints = 50
dataListOut = []
for i in range(dataPoints):
    dataListOut.append(1+i/10.0)
dataOut = 1.2345

ser = connectToSerial()

while(1):
    transmitList(dataListOut)
    time.sleep(0.2)
    dataListIn = receiveList(dataPoints)
    time.sleep(0.2)
    if dataListOut == dataListIn:
        print("List received OK")
    else:
        print("List reception failed")
    transmitData(dataOut)
    time.sleep(0.2)
    dataIn = receiveData()
    time.sleep(0.2)
    if dataOut == dataIn:
        print("Data received OK")
    else:
        print("Data reception failed")
