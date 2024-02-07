from machine import Pin, SPI, PWM
from time import sleep
from scroller import Scroller

# from dateHandling import dayText
import network
from uoauth2 import DeviceAuth

# For more information on how to create clients
# Look at: https://developers.google.com/identity/protocols/oauth2/limited-input-device

device_auth = DeviceAuth(
    client_id="648445354032-mv5p4b09hcj0116v57pnkmp42fn8m220.apps.googleusercontent.com",
    client_secret="9aeN3LGr0yq4TYjwGcfUVJKo",
    discovery_endpoint="https://accounts.google.com/.well-known/openid-configuration",
    scopes=list(["openid"]),
)

# Discover OpenID endpoints
device_auth.discover()

# Start authorization process
device_auth.authorize()

# Use the user-code and verification URL to show some UI to the user
# To complete the authorization process.
user_code = device_auth.user_code
verification_url = device_auth.verification_url

print(user_code, verification_url)

# Check for completed authorization
device_auth.check_authorization_complete()

# Fetch a valid access token
print(device_auth.token())

spi = SPI(1, baudrate=115000)
ss = Pin(15, Pin.OUT)
display = Scroller(spi, ss, 4)

event = {"start": {"date": "2023-10-23"}, "summary": "Event summary"}


# display.scroll_text(dayText(event))

sta_if = network.WLAN(network.STA_IF)
display.scroll_text(sta_if.ifconfig()[0])


display.brightness(1)  # adjust brightness 1 to 15
# Get version
filename = "version.txt"
f = open(filename, "r")
version = f.read().replace("\n", "")
display.scroll_text("Version: " + version)
sleep(2)


msg = "Under development!"
loop = 100
while loop > 0:
    display.scroll_text(msg)
    loop -= 1
