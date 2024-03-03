from machine import Pin, SPI, PWM, WDT
from time import sleep, sleep_ms
import ntptime
from max7219 import Matrix8x8
import sys

from dateHandling import dayText
from datetime import datetime
import network

import urequests as requests

spi = SPI(1, baudrate=115000)
ss = Pin(15, Pin.OUT)
display = Matrix8x8(spi, ss, 4)
# wdt = WDT()

# If modifying these scopes, delete the file token.json.
SCOPES = ["https://www.googleapis.com/auth/calendar.readonly"]



sta_if = network.WLAN(network.STA_IF)
display.scroll_text(sta_if.ifconfig()[0])

display.brightness(1)  # adjust brightness 1 to 15
# wdt.feed()

# Get version
filename = "version.txt"
f = open(filename, "r")
version = f.read().replace("\n", "")
# display.scroll_text("Version: " + version)

event = {"start": {"date": "2024-02-19T06:57:22.665"}, "summary": "Event summary"}
dt = datetime.fromisoformat(
        event["start"].get("dateTime", event["start"].get("date"))
    )
print(dt.now())

print(dayText(event))

event = {
    'start': {
        'date': '2024-02-25'
    },
    'summary': 'Vi jobber med låglønnsnæring'
}
display.scroll_text(dayText(event))

# wdt.feed()
sleep(1)
# wdt.feed()

msg = "I A i Åsane har vi både færøymål, låglønnsnæring og skjærgårdsøl"
display.scroll_text(msg)

while True:
    ntptime.settime()
    display.show_text(datetime.now().strftime("%H:%M:%S"))
    print(datetime.now().strftime("%H%M"))
    sleep_ms(200)
