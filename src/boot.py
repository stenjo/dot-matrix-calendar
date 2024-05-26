# This file is executed on every boot (including wake-boot from deepsleep)
import frozen_wifi_setup
# import frozen_setup
import frozen_micro_web_srv_2
import os, machine
import gc
gc.collect()

import network
import ntptime
import time

from DotMatrix import Matrix8x8
m = Matrix8x8(4,20,1)

m.clear()
m.init()
m.write("setup-")

from wifi_setup.wifi_setup import WiFiSetup

ws = WiFiSetup("dot-matrix-calendar")
sta = ws.connect_or_setup()
del ws

m.clear()
m.init()

#if needed, overwrite default time server
ntptime.host = "1.europe.pool.ntp.org"

try:
    print("Local time before synchronization: %s" %str(time.localtime()))
    #make sure to have internet connection
    ntptime.settime()
    print("Local time after synchronization: %s" %str(time.localtime()))
except:
    print("Error syncing time")

print(gc.mem_free())