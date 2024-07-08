import time
import network
from datetime import datetime, timezone, timedelta
from dateHandling import dayText
from DotMatrix import Matrix8x8
from Calendar import Calendar
import time
import network
from wifi_setup.wifi_setup import WiFiSetup
import gc

# (8x8 blocks, spi host, clock speed, CS pin)
m = Matrix8x8(4,20,1)
m.clear()
m.init()


# Get version
sta_if = network.WLAN(network.STA_IF)
filename = "version.txt"
f = open(filename, "r")
version = f.read().replace("\n", "")
m.marquee(sta_if.ifconfig()[0] + " " + version)
while not m.scroll():
    pass

c=Calendar(daysAhead=14)
c.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
c.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
c.parseURL('https://calendar.google.com/calendar/ical/ht3jlfaac5lfd6263ulfh4tql8%40group.calendar.google.com/public/basic.ics')

def displayClock(m):
    (_, _, _, hour, min, sec, _, _) = time.localtime()
    if sec % 15 == 0: return True
    if sec % 2 == 0:
        m.write("{:02}:{:02}".format(hour, min), True)
    else:
        m.write("{:02} {:02}".format(hour, min), True)
    return False

def reconnectIfNotConnected(sta_if):
    sta_if = network.WLAN(network.STA_IF); sta_if.active(True)
    if not sta_if.isconnected():
        while not sta_if.isconnected():
            sta_if.active(True)
            print("Trying to connect...")
            time.delay(1)
        ws = WiFiSetup("dot-matrix-calendar")
        sta = ws.connect_or_setup()
        del ws

scrollDone = False
clockDone = False
try:
    event = c.first()
    while True:
        if scrollDone is False: scrollDone = m.scroll(False)
        if scrollDone is True: clockDone = displayClock(m)
        if scrollDone and clockDone:
            if not event:
                items = c.refresh()
                event = c.first()
                reconnectIfNotConnected(sta_if)
                    
            if event:
                m.marquee(dayText(event))
            event = c.next()
            scrollDone = False
except KeyboardInterrupt:
    print("\nControl-C pressed. Cleaning up and exiting.")
finally:
    m.clear()
