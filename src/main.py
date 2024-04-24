from max7219 import Matrix8x8
from dateHandling import dayText
from datetime import datetime
import ujson
from Calendar import Calendar
import time

c=Calendar()
c.start(time.localtime())
c.end(datetime(2024, 6, 1, 0,0,0).timetuple())

c.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
c.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
# c.parseURL('https://calendar.google.com/calendar/ical/parterapeutene.no_e1or90m2lp6p523ma7u15v2pc0%40group.calendar.google.com/private-95204a6ebd527a9ba3d5004beeb57b74/basic.ics')
event = c.first()

print(event)  # Should print a tuple like ("Test Event", "20230412T160000Z")
print(dayText(event))

m = Matrix8x8()

m.clear()
m.init()
m.marquee(dayText(event))
while event:
    done = False
    while done == False:
        done = m.scroll(False)
    event = c.next()
    if not event:
        event = c.first()
    m.marquee(dayText(event))

m.test()
done = False
while done == False:
    done = m.scroll(False)
