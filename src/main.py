from max7219 import Matrix8x8
from dateHandling import dayText
from datetime import datetime
import ujson
from Calendar import Calendar
import time

c=Calendar('f2-calendar_p_q_sprint_feature.ics', 
           start=time.localtime(), 
           end=datetime(2024, 8, 1, 0,0,0).timetuple())
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
