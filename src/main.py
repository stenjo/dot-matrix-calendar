from machine import Pin, SPI, PWM, WDT
from time import sleep
from scroller import Scroller
import sys

from dateHandling import dayText
from datetime_ext import datetime
import network

spi = SPI(1, baudrate=115000)
ss = Pin(15, Pin.OUT)
display = Scroller(spi, ss, 4)
# wdt = WDT()


event = {"start": {"date": "2023-10-23T06:57:22.665"}, "summary": "Event summary"}
dt = datetime.fromisoformat(
        event["start"].get("dateTime", event["start"].get("date"))
    )
print(dt.now())

print(dt.timetuple())

print(dt.strftime("%Y-%m-%d"))

print(dayText(event))
# display.scroll_text(dayText(event))

sta_if = network.WLAN(network.STA_IF)
display.scroll_text(sta_if.ifconfig()[0])

display.brightness(1)  # adjust brightness 1 to 15
# wdt.feed()

# Get version
filename = "version.txt"
f = open(filename, "r")
version = f.read().replace("\n", "")
display.scroll_text("Version: " + version)
# wdt.feed()
sleep(1)
# wdt.feed()

msg = "Under development!"
loop = 100
while loop > 0:
    # wdt.feed()
    display.scroll_text(msg)
    loop -= 1
