#!/usr/bin/env python3

import sys
import os
import argparse
import serial
import readline
import atexit
from threading import Thread, Lock
from datetime import datetime
import math
import time
import traceback
import json
from socket import *


DEVICES = [
    "/dev/cu.usbmodem497A215F34301",
    "/dev/ttyACM0"
]
PATH = os.path.dirname(__file__)


class Emu:
    mutex = Lock()

    def command(self, cmd):
        print(f"<< {cmd}")
        ans = self.process(cmd)
        print(f">> {ans}")
        return ans

    def process(self, cmd):
        with self.mutex:
            return self._process(cmd)

    def _process(self, cmd):
        def vl3(val):
            s = str(int(abs(val)))
            while len(s) < 3:
                s = '0'+s
            return s
        if cmd == "status":
            tm = datetime.now().timestamp()
            v1 = math.sin(tm)*234
            v2 = math.cos(tm)*99
            v3 = math.sin(tm+math.pi/0.3)*140
            return f"{vl3(v1)} {vl3(v2)} ERX {vl3(v3)}"
        return cmd + " ok"


class Device(Emu):

    def __init__(self, dev):
        Emu.__init__(self)
        self.f = serial.Serial(dev, 115200, parity=serial.PARITY_NONE,
                               stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)

    def _process(self, cmd):
        Emu.process(cmd)
        self.f.write(cmd.encode())
        return self.readline(self.f)

    def readline(self, f):
        ret = ""
        while True:
            c = f.read(1)
            ret += c.decode("utf-8")
            if c == b'\n':
                return ret


class PlotThread(Thread):
    def __init__(self, dev):
        Thread.__init__(self)
        self.dev = dev
        self._cont = True
        self.socket = socket(AF_INET, SOCK_DGRAM)
        with open(os.path.join(PATH, "conf.json")) as f:
            conf = json.load(f)
        self.addr = ("127.0.0.1", conf['port'])
        os.system(os.path.join(PATH, "plot.py")+"&")

    def join(self):
        self._cont = False
        Thread.join(self)

    def values(self, arr):
        ret = [0, 0, 0, 0]
        for i in range(min(4, len(arr))):
            if arr[i] <= '9':
                ret[i] = int(arr[i])
        return ret

    def run(self):
        while self._cont:
            time.sleep(1)
            status = self.dev.process("status")
            tm = datetime.now()
            data = [tm.timestamp()] + self.values(status.split())
            self.socket.sendto(json.dumps(data).encode(), self.addr)


def process(dev, cmd, opts):
    plot = None
    if opts.plot:
        # start plot proc and thread
        plot = PlotThread(dev)
        plot.start()
    if cmd.strip() != '':
        if os.path.isfile(cmd):
            cmds = []
            with open(cmd) as f:
                cmds = f.read().split("\n")
            for x in cmds:
                dev.command(x)
        else:
            dev.command(cmd)
        if not opts.plot:
            return
    while True:
        cmd = input("> ")
        if cmd in ('q', 'e', 'quit', 'exit'):
            break
        try:
            dev.command(cmd)
        except Exception as e:
            traceback.print_exc()
    if plot:
        plot.join()


def getDev(optdev):
    if optdev == "emu":
        print("Running on emulator")
        return Emu()
    if optdev:
        return Device(optdev)
    for x in DEVICES:
        if os.path.exists(x):
            return Device(x)
    raise Exception("Device not found")


def main():
    histfile = os.path.join(os.path.expanduser("~"), ".termhist")
    try:
        readline.read_history_file(histfile)
        readline.set_history_length(100)
    except FileNotFoundError:
        pass
    atexit.register(readline.write_history_file, histfile)
    parser = argparse.ArgumentParser()
    parser.add_argument("--verbose", "-v", action="store_true")
    parser.add_argument("--device", "-d", default=None)
    parser.add_argument("--plot", "-p", action="store_true")
    opts, args = parser.parse_known_args()
    process(getDev(opts.device), ' '.join(args), opts)


if __name__ == "__main__":
    main()
