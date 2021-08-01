#!/usr/bin/env python3

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import json
import os
import random
from datetime import datetime
from socketserver import *
from threading import Lock, Thread


class Data:
    def __init__(self):
        self.x = []
        self.y = [[], [], [], []]
        self.current = [0, 0, 0, 0]


D = Data()
mutex = Lock()


def plotAnim(i):
    mutex.acquire()
    try:
        plt.cla()
        plt.plot(D.x, D.y[0])
        plt.plot(D.x, D.y[1])
        plt.plot(D.x, D.y[2])
        plt.plot(D.x, D.y[3])
        plt.xlabel('time')
        plt.ylabel('values')
        plt.title(f"{D.current}")
        plt.legend(["P1", "T1", "T2", "P2"])
    finally:
        mutex.release()


class UDPHandler(DatagramRequestHandler):

    def handle(self):
        mutex.acquire()
        try:
            data = json.loads(self.request[0].decode())
            D.x += [datetime.fromtimestamp(data[0])]
            D.current = data[1:]
            D.y[0] += [data[1]]
            D.y[1] += [data[2]]
            D.y[2] += [data[3]]
            D.y[3] += [data[4]]
        finally:
            mutex.release()


path = os.path.dirname(__file__)
with open(os.path.join(path, "conf.json")) as f:
    conf = json.load(f)
udp = UDPServer(("127.0.0.1", conf['port']), UDPHandler)
server_thread = Thread(target=udp.serve_forever)
server_thread.daemon = True
server_thread.start()

anim = FuncAnimation(plt.gcf(), plotAnim, interval=500)
plt.tight_layout()
plt.show()

udp.shutdown()
server_thread.join()
udp.server_close()
