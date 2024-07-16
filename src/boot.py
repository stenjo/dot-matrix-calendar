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
m = Matrix8x8(host=1, cs=5)

m.clear()
m.init()
m.write("setup-")

from wifi_setup.wifi_setup import WiFiSetup

ws = WiFiSetup("dot-matrix-calendar")
sta = ws.connect_or_setup()
del ws

m.clear()
m.init()

done = False
#if needed, overwrite default time server
ntptime.host = "1.europe.pool.ntp.org"
print("Local time before synchronization: %s" %str(time.localtime()))
while done == False:
    
    try:
        done = True
        #make sure to have internet connection
        ntptime.settime()
        
    except:
        done = False        
        print("Error syncing time. Trying again ...")
        time.sleep(5)

    finally:
        print("Local time after synchronization: %s" %str(time.localtime()))

print(gc.mem_free())