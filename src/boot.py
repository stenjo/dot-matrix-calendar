# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import os, machine
#os.dupterm(None, 1) # disable REPL on UART(0)
import gc
#import webrepl
#webrepl.start()
gc.collect()

# boot.py -- run on boot-up
import os
import network
import mpy_env
# from mpy_env import load_env, get_env, put_env

mpy_env.load_env()


def do_connect():
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print("")
        print("--------------------------------")
        print("connecting to network...")
        sta_if.active(True)
        sta_if.connect(mpy_env.get_env("WIFI_SSID"), mpy_env.get_env("WIFI_PASSWD"))
        while not sta_if.isconnected():
            pass
    print("network config:", sta_if.ifconfig())


do_connect()