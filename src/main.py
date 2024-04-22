from max7219 import Matrix8x8
from ics_parser import ICS
from dateHandling import dayText
from datetime import datetime
import ujson

i=ICS()
f = open('f2-calendar_p_q_sprint_feature.ics')
event_tuple=i.parse(f.read())
f.close()

def convert_dtstart_iso(dtstart):
    # Assuming the format of dtstart is '20230412T165722Z'
    # We remove the 'Z' as it indicates UTC and 'fromisoformat' does not support 'Z'
    dtstart = dtstart.rstrip('Z')
    date_part = dtstart[:8]
    time_part = dtstart[9:]
    
    # Insert hyphens and colons to match ISO 8601 format
    date_iso = "{}-{}-{}".format(date_part[:4], date_part[4:6], date_part[6:])
    time_iso = "{}:{}:{}".format(time_part[:2], time_part[2:4], time_part[4:])

    return "{}T{}".format(date_iso, time_iso)


def toDict(event_tuple):
    return {
        "start": {"date": convert_dtstart_iso(event_tuple[1])},
        "summary": event_tuple[0]
    }

# Usage
event = toDict(event_tuple)

print(event)  # Should print a tuple like ("Test Event", "20230412T160000Z")
print(dayText(event))

m = Matrix8x8()

m.clear()
m.init()
m.marquee(dayText(event))
while event_tuple:
    done = False
    while done == False:
        done = m.scroll(False)
    event_tuple = i.getNext()
    event = toDict(event_tuple)
    m.marquee(dayText(event))

m.test()
done = False
while done == False:
    done = m.scroll(False)
