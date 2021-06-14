#!/usr/bin/env python3
'''
term -- shortdesc

term is a description

It defines classes_and_methods

@author:     user_name

@copyright:  2020 organization_name. All rights reserved.

@license:    license

@contact:    user_email
@deffield    updated: Updated
'''

import sys
import os
import argparse
import logging
import serial


DEVICES = [
	"/dev/cu.usbmodem497A215F34301",
	"/dev/ttyACM0"
]

def readline(f):
    ret = ""
    while True:
        c = f.read(1)
        ret += c.decode("utf-8")
        if c == b'\n':
            return ret


def procDev(dev, cmd):
    f=serial.Serial(dev, 115200, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)
    print(f"<< {cmd}")
    f.write(cmd.encode())
    ans = readline(f)
    print(f">> {ans}")


def getDev(optdev):
	if optdev:
		return optdev
	for x in DEVICES:
		if os.path.exists(x):
			return x
	raise Exception("Device not found")	

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--device", "-d", default=None)
    parser.add_argument("--verbose", "-v", action="store_true")
    opts, args = parser.parse_known_args()
    procDev(getDev(opts.device), ' '.join(args))


if __name__ == "__main__":
    main()
