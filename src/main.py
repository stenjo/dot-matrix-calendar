from max7219 import Matrix8x8
from uICAL import RRule

m = Matrix8x8()

r=RRule("Rule text", "2024-04-10:12:57:34")
# m.test()
m.clear()
m.init()
m.write("øæå")
m.marquee("Forza Standard Pipe Tracking system (SPTS) and its imminent release of new dressing and architecture!")
while True:
    if m.scroll(True):
        print("Loop done!")

