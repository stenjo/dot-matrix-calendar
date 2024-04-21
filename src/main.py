from max7219 import Matrix8x8
from ics_parser import ICS

ics_data = """BEGIN:VCALENDAR
VERSION:2.0
BEGIN:VEVENT
SUMMARY:Meeting with John
DTSTART:20230412T160000Z
END:VEVENT
END:VCALENDAR"""

i=ICS()
event=i.parse_file("f2-calendar_p_q_sprint_feature.ics")

print(event)  # Should print a tuple like ("Meeting with John", "20230412T160000Z")

m = Matrix8x8()

m.clear()
m.init()
m.marquee(event[0])
done = False
while done == False:
    done = m.scroll(False)

m.test()
done = False
while done == False:
    done = m.scroll(False)
