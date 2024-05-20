# This file is executed on every boot (including wake-boot from deepsleep)
# import frozen_wifi_setup.py
import frozen_wifi_setup
import frozen_micro_web_srv_2
import os, machine
import gc
gc.collect()

import network
import ntptime
import time

from wifi_setup.wifi_setup import WiFiSetup

ws = WiFiSetup("dot-matrix-calendar")
sta = ws.connect_or_setup()
del ws


#if needed, overwrite default time server
ntptime.host = "1.europe.pool.ntp.org"

try:
    print("Local time before synchronization: %s" %str(time.localtime()))
    #make sure to have internet connection
    ntptime.settime()
    print("Local time after synchronization: %s" %str(time.localtime()))
except:
    print("Error syncing time")
