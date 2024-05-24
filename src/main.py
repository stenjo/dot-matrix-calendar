import time
import network
from datetime import datetime, timezone, timedelta
from dateHandling import dayText
from DotMatrix import Matrix8x8
from Calendar import Calendar
import time
import network
from wifi_setup.wifi_setup import WiFiSetup

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

(year, month, day, hour, min, sec, _, _) = time.localtime()
print("{}:{:02}:{:02}".format(hour, min, sec))
m.write("{}/{} {:02}:{:02}".format(day, month, hour, min), True)
time.sleep(3)  # import time(1)

c=Calendar()
c.start(time.gmtime())
c.end((datetime.now() + timedelta(30)).timetuple())

c.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
c.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
# c.parseURL('https://calendar.google.com/calendar/ical/c_eae215482ecd0bf862ff838cb81657e12281bff2f104c0986f78b20d90e4917c%40group.calendar.google.com/private-5cf4e55067a529ddd245d8a2f15a5e49/basic.ics')

event = c.first()
if event: m.marquee(dayText(event))
done = False

def displayClock(m):
    (_, _, _, hour, min, sec, _, _) = time.localtime()
    while sec != 0:
        if sec % 2 == 0:
            m.write("{:02}:{:02}".format(hour, min), True)
        else:
            m.write("{:02} {:02}".format(hour, min), True)
        (_, _, _, hour, min, sec, _, _) = time.localtime()

try:
    while True:
        done = m.scroll(False)
        if done:
            displayClock(m)
            event = c.next()
            if not event:
                items = c.refresh(time.gmtime(), (datetime.now() + timedelta(30)).timetuple())
                event = c.first()
                # print("Refetched {:02} calendar items.".format(items))
                sta_if = network.WLAN(network.STA_IF); sta_if.active(True)
                ws = WiFiSetup("dot-matrix-calendar")
                sta = ws.connect_or_setup()
                del ws
            if event:
                m.marquee(dayText(event))
except KeyboardInterrupt:
    print("\nControl-C pressed. Cleaning up and exiting.")
finally:
    m.clear()
