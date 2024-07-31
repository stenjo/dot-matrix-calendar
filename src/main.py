from datetime import datetime, timezone, timedelta
from dateHandling import dayText
from Calendar import Calendar
from Clock import Clock
import gc
import utils
import esp32
import time

gc.collect()
print("Free mem: ",gc.mem_free())


matrix.clear()

# Get version
utils.show_ip_and_version(matrix)
clock = Clock(15, False)

calendar=Calendar(daysAhead=7)
calendar.parseURL('webcal://files-f2.motorsportcalendars.com/no/f2-calendar_p_q_sprint_feature.ics')
calendar.parseURL('webcal://files-f3.motorsportcalendars.com/no/f3-calendar_p_q_sprint_feature.ics')
calendar.parseURL('https://calendar.google.com/calendar/ical/parterapeutene.no_e1or90m2lp6p523ma7u15v2pc0%40group.calendar.google.com/public/basic.ics')
calendar.parseURL('https://calendar.google.com/calendar/ical/c_eae215482ecd0bf862ff838cb81657e12281bff2f104c0986f78b20d90e4917c%40group.calendar.google.com/private-5cf4e55067a529ddd245d8a2f15a5e49/basic.ics')
calendar.parseURL('https://calendar.google.com/calendar/ical/ht3jlfaac5lfd6263ulfh4tql8%40group.calendar.google.com/public/basic.ics')

scrollDone = False
print("Free mem: ",gc.mem_free())

try:
    event = calendar.first()
    while True:
        if scrollDone is False: scrollDone = matrix.scroll(False)
        if scrollDone is True: clockDone = clock.display(matrix)
        if scrollDone and clockDone:
            if not event:
                items = calendar.refresh()
                print("Free mem: ",gc.mem_free())
                event = calendar.first()
                utils.reconnectIfNotConnected()
                utils.boardTemp()
            
            if event:
                matrix.marquee(dayText(event, datetime.now(timezone.cet)))
                
            event = calendar.next()
            scrollDone = False
except KeyboardInterrupt:
    print("\nControl-C pressed. Cleaning up and exiting.")
finally:
    matrix.clear()
