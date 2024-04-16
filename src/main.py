from max7219 import Matrix8x8
import ics_parser

ics_data = """BEGIN:VCALENDAR
VERSION:2.0
BEGIN:VEVENT
SUMMARY:Meeting with John
DTSTART:20230412T160000Z
END:VEVENT
END:VCALENDAR"""

event = ics_parser.parse(ics_data)

print(event)  # Should print a tuple like ("Meeting with John", "20230412T160000Z")
m = Matrix8x8()

m.clear()
m.init()
m.test()
done = False
while done == False:
    done = m.scroll(False)

m.marquee("Forza Standard Pipe Tracking System (SPTS) and its imminent release of new dressing and architecture!")
while True:
    if m.scroll(True):
        print("Loop done!")

