# boot.py -- run on boot-up
import os
import network
from mpy_env import load_env, get_env, put_env

load_env()


def do_connect():
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print("--------------------------------")
        print("connecting to network...")
        sta_if.active(True)
        sta_if.connect(get_env("WIFI_SSID"), get_env("WIFI_PASSWD"))
        while not sta_if.isconnected():
            pass
    print("network config:", sta_if.ifconfig())


do_connect()
