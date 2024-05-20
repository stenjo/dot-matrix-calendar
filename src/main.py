from DotMatrix import Matrix8x8
from dateHandling import dayText
from datetime import datetime, timezone, timedelta
import ujson
import ssl
from Calendar import Calendar
import time
import network

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

(year, month, day, hour, min, sec, _, _) = time.localtime()
print("{}:{:02}:{:02}".format(hour, min, sec))
m.write("{}/{} {:02}:{:02}".format(day, month, hour, min), True)
time.sleep(3)  # import time(1)

c=Calendar()
c.start(time.gmtime())
c.end((datetime.now() + timedelta(30)).timetuple())

c.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
c.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
# c.parseURL('https://calendar.google.com/calendar/ical/c_bbd06d1ace0398da9397cae670201961dc43e8e1f37c017e5261650ed94c9192%40group.calendar.google.com/public/basic.ics')
event = c.first()

print(event)  # Should print a tuple like ("Test Event", "20230412T160000Z")
print(dayText(event))


event = c.first()
m.marquee(dayText(event))
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
                c.refresh(time.gmtime(), (datetime.now() + timedelta(30)).timetuple())
                event = c.first()
            m.marquee(dayText(event))
except KeyboardInterrupt:
    print("\nControl-C pressed. Cleaning up and exiting.")
finally:
    m.clear()
