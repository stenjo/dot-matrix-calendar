from datetime import datetime, timezone, timedelta
from dateHandling import dayText
from DotMatrix import Matrix8x8
from Calendar import Calendar
from Clock import Clock
import gc
import utils

gc.collect()
print("Free mem: ",gc.mem_free())


m.clear()

# Get version
utils.show_ip_and_version(m)

c=Calendar(daysAhead=7)
c.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
c.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
c.parseURL('https://calendar.google.com/calendar/ical/parterapeutene.no_e1or90m2lp6p523ma7u15v2pc0%40group.calendar.google.com/public/basic.ics')

scrollDone = False
clock = Clock(15)
print("Free mem: ",gc.mem_free())

try:
    event = c.first()
    while True:
        if scrollDone is False: scrollDone = m.scroll(False)
        if scrollDone is True: clockDone = clock.display(m)
        if scrollDone and clockDone:
            if not event:
                items = c.refresh()
                print("Free mem: ",gc.mem_free())
                event = c.first()
                utils.reconnectIfNotConnected()

            if event:
                m.marquee(dayText(event))
                
            event = c.next()
            scrollDone = False
except KeyboardInterrupt:
    print("\nControl-C pressed. Cleaning up and exiting.")
finally:
    m.clear()
