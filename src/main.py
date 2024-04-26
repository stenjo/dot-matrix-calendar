from max7219 import Matrix8x8
from dateHandling import dayText
from datetime import datetime
import ujson
import ssl
from Calendar import Calendar
import time

c=Calendar()
c.start(time.localtime())
c.end(datetime(2024, 6, 1, 0,0,0).timetuple())

c.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
c.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
c.parseURL('https://calendar.google.com/calendar/ical/c_bbd06d1ace0398da9397cae670201961dc43e8e1f37c017e5261650ed94c9192%40group.calendar.google.com/public/basic.ics')
event = c.first()

print(event)  # Should print a tuple like ("Test Event", "20230412T160000Z")
print(dayText(event))

# (8x8 blocks, spi host, clock speed, CS pin)
m = Matrix8x8(16)

m.clear()
m.init()
seconds = 60
while seconds > 0:
    tm = time.localtime()
    m.write("     {}:{:02}:{:02}".format(tm[3], tm[4], tm[5]))
    time.sleep(1)
    seconds -= 1

m.marquee(dayText(event))
while event:
    done = False
    while done == False:
        done = m.scroll(False)
    event = c.next()
    if not event:
        seconds = 60
        while seconds > 0:
            tm = time.localtime()
            m.write("   {} : {:02} : {:02}".format(tm[3], tm[4], tm[5]))
            time.sleep(0.5)
            seconds -= 1
        event = c.first()
    m.marquee(dayText(event))

m.test()
done = False
while done == False:
    done = m.scroll(False)
