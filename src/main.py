from machine import Pin, SPI, PWM
from time import sleep
from scroller import Scroller
from dateHandling import dayText


# from google_auth_oauthlib.flow import InstalledAppFlow
import network

spi = SPI(1, baudrate=115000)
ss = Pin(15, Pin.OUT)


display = Scroller(spi, ss, 4)

event = {
    'start': {
        'date': '2023-10-23'
    },
    'summary': 'Event summary'
}


# display.scroll_text(dayText(event))

sta_if = network.WLAN(network.STA_IF)
display.scroll_text(sta_if.ifconfig()[0])


display.brightness(1)  # adjust brightness 1 to 15
# Get version
filename = 'version.txt'
f = open(filename, 'r')
version = f.read().replace('\n','')
display.scroll_text("Version: " + version)
sleep(2)


msg = "Under development!"
loop = 100
while loop > 0:
    display.scroll_text(msg)
    loop -= 1
