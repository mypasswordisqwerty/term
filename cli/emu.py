#!/usr/bin/env python3
import sys
import os
from threading import Thread, Lock
from datetime import datetime
import math
import time
import traceback
import json
from socket import *
import random

PATH = os.path.dirname(__file__)


class PIDEmu:
    P = 0.01
    I = 0.3
    D = 0.1
    SIU = 0.5
    SID = -0.5
    CTIME = 10

    def __init__(self, targ):
        self.ptime = time.time()
        self.ctime = 0
        self.TARG = targ
        self.t = 0
        self.p = 0.0
        self.ep = 0
        self.dy = 0
        self.si = 0

    def recalc(self, dt):
        self.t = max(0, self.t-dt - dt*random.random())
        self.t += self.p*5
        # pid
        ec = self.TARG - self.t
        self.si = max(self.SID, min(self.si + ec*dt, self.SIU))
        de = (ec - self.ep)/dt

        self.dy = ec * self.P
        self.dy += self.si * self.I
        self.dy += de * self.D

        self.ep = ec

        self.ctime -= dt
        if self.ctime <= 0:
            self.ctime = self.CTIME
            # power ajust
            self.p = max(0, min(self.p+self.dy, 1.0))
            self.dy = 0

    def command(self, cmd):
        self.TARG = int(cmd)

    def process(self, cmd):
        def vl3(val):
            s = str(int(abs(val)))
            while len(s) < 3:
                s = '0'+s
            return s
        now = time.time()
        dt = now - self.ptime
        self.ptime = now
        self.recalc(dt)
        return f"{vl3(int(self.p*100))} {vl3(int(self.t))} {vl3(self.TARG)} ERX"


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


def main():
    dev = PIDEmu(int(sys.argv[1]) if len(sys.argv) > 1 else 0)
    plot = PlotThread(dev)
    plot.start()
    while True:
        cmd = input("> ")
        if cmd in ('q', 'e', 'quit', 'exit'):
            break
        try:
            dev.command(cmd)
        except Exception as e:
            traceback.print_exc()
    plot.join()


if __name__ == "__main__":
    main()
