from DotMatrix import Matrix8x8
from dateHandling import dayText
from datetime import datetime
import ujson
import ssl
from Calendar import Calendar
import time
import network

c=Calendar()
c.start(time.localtime())
c.end(datetime(2024, 6, 1, 0,0,0).timetuple())

c.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
c.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
# c.parseURL('https://calendar.google.com/calendar/ical/c_bbd06d1ace0398da9397cae670201961dc43e8e1f37c017e5261650ed94c9192%40group.calendar.google.com/public/basic.ics')
event = c.first()

print(event)  # Should print a tuple like ("Test Event", "20230412T160000Z")
print(dayText(event))

# (8x8 blocks, spi host, clock speed, CS pin)
m = Matrix8x8(8,20)

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
event = c.first()
m.marquee(dayText(event))
done = False
while True:
    done = m.scroll(False)
    if done:
        (year, month, day, hour, min, sec, _, _) = time.localtime()
        while sec != 0:
            if sec % 2 == 0:
                m.write("{}/{} {:02}:{:02}".format(day, month, hour, min), True)
            else:
                m.write("{}/{} {:02} {:02}".format(day, month, hour, min), True)
            (year, month, day, hour, min, sec, _, _) = time.localtime()
        event = c.next()
        if not event:
            event = c.first()
        m.marquee(dayText(event))

m.test()
done = False
while done == False:
    done = m.scroll(False)
