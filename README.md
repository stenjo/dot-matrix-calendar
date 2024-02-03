# dot-matrix-calendar

Displaying a marquee list of events from a google calendar on a Wemos D1 mini and a max7219-driven dot matrix led using Micropython

## Setup

Add a env.json in the format:

```json
{
    "WIFI_SSID":"<network ssid>",
    "WIFI_PASSWD":"<network password>"
}
```

## Credits

[MicroPython Max7219 8x8 LED Matrix](https://pypi.org/project/micropython-max7219/): Leo Spratt ([enchant97](https://pypi.org/user/enchant97/))
[micropython-env](https://github.com/ShenTengTu/micropython-env/tree/master): Shen-Teng Tu ([ShengTengtu](https://github.com/ShenTengTu))