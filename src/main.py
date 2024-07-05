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
gc.collect()

# (8x8 blocks, spi host, clock speed, CS pin)
m = Matrix8x8(8,20,1)

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

print(gc.mem_free())

(year, month, day, hour, min, sec, _, _) = time.localtime()
print("{}:{:02}:{:02}".format(hour, min, sec))
m.write("{}/{}-{}".format(day, month, year), True)
time.sleep(3)  # import time(1)

c=Calendar()
c.start(time.gmtime())
c.end((datetime.now() + timedelta(7)).timetuple())

# c.parseURL('https://calendar.google.com/calendar/ical/parterapeutene.no_e1or90m2lp6p523ma7u15v2pc0%40group.calendar.google.com/public/basic.ics')
print(gc.mem_free())
c.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
print(gc.mem_free())
c.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
print(gc.mem_free())
# c.parseURL('https://calendar.google.com/calendar/ical/no.norwegian%23holiday%40group.v.calendar.google.com/public/basic.ics')
# print(gc.mem_free())
# c.parseURL('https://calendar.google.com/calendar/ical/i_213.236.150.86%23sunrise%40group.v.calendar.google.com/public/basic.ics')
done = False

def displayClock(m):
    (_, _, _, hour, min, sec, _, _) = time.localtime()
    while sec % 15 != 0:
        if sec % 2 == 0:
            m.write("{:02}:{:02}".format(hour, min), True)
        else:
            m.write("{:02} {:02}".format(hour, min), True)
        (_, _, _, hour, min, sec, _, _) = time.localtime()

try:
    event = c.first()
    if event: m.marquee(dayText(event))
    
finally:
    displayClock(m)
    
try:
    while True:
        done = m.scroll(False)
        if done:
            displayClock(m)
            event = c.next()
            if not event:
                gc.collect()
                items = c.refresh(time.gmtime(), (datetime.now() + timedelta(30)).timetuple())
                print(gc.mem_free())
                event = c.first()
                # print("Refetched {:02} calendar items.".format(items))
                sta_if = network.WLAN(network.STA_IF); sta_if.active(True)
                if not sta_if.isconnected():
                    while not sta_if.isconnected():
                        sta_if.active(True)
                        print("Trying to connect...")
                        time.delay(1)

                    ws = WiFiSetup("dot-matrix-calendar")
                    sta = ws.connect_or_setup()
                    del ws
                    
            if event:
                m.marquee(dayText(event))
except KeyboardInterrupt:
    print("\nControl-C pressed. Cleaning up and exiting.")
finally:
    m.clear()
