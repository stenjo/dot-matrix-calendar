from machine import Pin, SPI, PWM
from max7219 import Matrix8x8
from time import sleep_ms, sleep
from math import sin, pi

# from google_auth_oauthlib.flow import InstalledAppFlow
import network

spi = SPI(1, baudrate=115000)
ss = Pin(15, Pin.OUT)


class Matrix8x8Ext(Matrix8x8):
    def scroll_text(self, s, ms_delay=100):
        s_width = len(s) * 8
        n_pixels = self._num * 8
        for x in range(n_pixels, -s_width, -1):
            self.zero()
            self.text(s, x)
            self.show()
            sleep_ms(ms_delay)
            self.show()
        return s_width


def pulse(l, t):
    for i in range(20):
        l.duty(int(sin(i / 10 * pi) * 500 + 500))
        sleep_ms(t)


led = PWM(Pin(2), freq=1000)
display = Matrix8x8Ext(spi, ss, 4)

sta_if = network.WLAN(network.STA_IF)
display.scroll_text(sta_if.ifconfig()[0], 50)
# display.show()
sleep(2)

pulse(led, 50)

display.brightness(1)  # adjust brightness 1 to 15
display.text("CODE")
display.show()
sleep(2)

pulse(led, 50)

msg = "Finally, this works!"
loop = 100
while loop > 0:
    display.scroll_text(msg, 50)
    # display.show()
    for i in range(10):
        pulse(led, 50)

    loop -= 1
