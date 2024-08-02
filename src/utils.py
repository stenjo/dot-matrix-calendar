import network
from wifi_setup.wifi_setup import WiFiSetup
import time
import esp32

def show_ip_and_version(matrix):
    # Get version
    sta_if = network.WLAN(network.STA_IF)
    filename = "version.txt"
    f = open(filename, "r")
    version = f.read().replace("\n", "")
    matrix.marquee(sta_if.ifconfig()[0] + " " + version)
    while not matrix.scroll():
        pass

def reconnectIfNotConnected():
    sta_if = network.WLAN(network.STA_IF); sta_if.active(True)
    if not sta_if.isconnected():
        while not sta_if.isconnected():
            sta_if.active(True)
            print("Trying to connect...")
            time.delay(1)
        ws = WiFiSetup("dot-matrix-calendar")
        sta = ws.connect_or_setup()
        del ws
        
def boardTemp():

    if hasattr(esp32, 'raw_temperature'):
        tf = esp32.raw_temperature()
        tc = (tf-32.0)/1.8
        print("T = {0:4d} deg F or {1:5.1f}  deg C".format(tf,tc))
    else:
        print("T = {0:4d} deg C".format(esp32.mcu_temperature()))
        
