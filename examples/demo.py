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

header = chr(0x7E)
footer = chr(0x7E)

def connect_to_serial():
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

def check_message(message, check):
    xor_check = 0x00
    for i in range(len(message)):
        for j in range(len(message[i])):
            xor_check = xor_check^ord(message[i][j])
    xor_check = chr(xor_check)
    if(xor_check==check):
        return True
    else:
        return False

def get_message(data_points):
    message = ''
    message_size = data_points*4
    in_byte = chr(0x00)
    i = 0
    while(ser.read()!=header): None
    while(ser.inWaiting()>0):
        in_byte = ser.read()
        if(in_byte!=footer and i<message_size):
            message += in_byte
            i += 1
        elif(in_byte!=footer and i==message_size):
            check = in_byte
        elif(in_byte==footer):
            return message, check

def read_data():
    message, check = get_message(1)
    if(check_message(message,check)):
        data = round(struct.unpack('f',message)[0],4)
        return data
    else:
        return None

def read_list(data_points):
    message, check = get_message(data_points)
    data_list = []
    i = 0
    if(check_message(message,check)):
        for j in range(data_points):
            data = ''
            for k in range(4):
                data += message[i]
                i += 1
            data = round(struct.unpack('f',data)[0],4)
            data_list.append(data)
        return data_list
    else:
        return None

def write_data(data_out):
    check = 0x00
    data = struct.pack('f',float(data_out))
    for i in range(len(data)):
        check = check^ord(data[i])
    check = chr(check)
    message = header+data+check+footer
    ser.write(message)

def write_list(list_out):
    check = 0x00
    message = header
    for i in range(len(list_out)):
        data = struct.pack('f',float(list_out[i]))
        message += data
        for j in range(len(data)):
            check = check^ord(data[j])
    check = chr(check)
    message += check + footer
    ser.write(message)

data_points = 50
list_out = []
for i in range(data_points):
    list_out.append(1+i/10.0)
data_out = 1.2345

ser = connect_to_serial()

while(1):
    write_list(list_out)
    time.sleep(0.2)
    list_in = read_list(data_points)
    time.sleep(0.2)
    if list_out == list_in:
        print("List received OK")
    else:
        print("List reception failed")
    write_data(data_out)
    time.sleep(0.2)
    data_in = read_data()
    time.sleep(0.2)
    if data_out == data_in:
        print("Data received OK")
    else:
        print("Data reception failed")
