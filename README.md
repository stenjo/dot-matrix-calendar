# dot-matrix-calendar

Displaying a marquee list of events from one or more ics calendar feeds on an ESP32 based Lolin S2 Mini and a max7219-driven dot matrix led using Micropython.

## Getting started

For this project to work, you need the following:

- [Wemos Lolin S2 Mini](https://elkim.no/produkt/esp32-s2-mini-v1-0-0/)
- [8 module 8x8 dot matrix led displays with an SPI interface](https://www.aliexpress.com/item/1005006246992859.html?channel=twinner)
- 3D printed [stand](stand/README.md)

### Wiring

Connect the dot matrix module to the S2 mini:

| S2 mini | Dot Matrix | signal      |
|---------|------------|-------------|
| 3V3     | VCC        | Power       |
| GND     | GND        | Ground      |
| GPIO 11 | DIN        | Data        |
| GPIO 5  | CS         | Chip select |
| GPIO 7  | CLK        | Clock       |

![Wiring](wiring.png)

### Load binary


## Dependencies
|-----------------|------------------|
| `esptool.py` | https://docs.espressif.com/projects/esptool/en/latest/esp32/ |
| xtensa gcc | see lib/micropython/ports/esp8266/README.md |
| freezefs | https://github.com/bixb922/freezefs |

Make sure micropython lib is upated

``` bash
git submodule update --init lib/micropython
make -C lib/micropython/mpy-cross
make -C ports/esp8266 submodules

git submodule update --init modules/micropython-wifi-setup
```

Update wifi setup library
``` bash
git submodule update --init modules/micropython-wifi-setup
python -m freezefs modules/micropython-wifi-setup/lib/wifi_setup modules/frozen_wifi_setup.py -ov always
python -m freezefs modules/micropython-wifi-setup/lib/frozen_micro_web_srv_2 modules/frozen_frozen_micro_web_srv_2.py -ov always
```

## Building for ESP8266
```bash
cd boards/ESP8266_GENERIC
make
```

Build firmware for downloading to device using docker:

```bash
docker run --rm -v $HOME:$HOME -u $UID -w $PWD larsks/esp-open-sdk make PYTHON=python3
```

## Setup

Add a env.json in the format:

```json
{
    "WIFI_SSID":"<network ssid>",
    "WIFI_PASSWD":"<network password>"
}
```

## Credits

[MicroPython Max7219 8x8 LED Matrix]()
[micropython-env](https://github.com/ShenTengTu/micropython-env/tree/master): Shen-Teng Tu ([ShengTengtu](https://github.com/ShenTengTu))
[MicroPython WiFi setup](https://github.com/george-hawkins/micropython-wifi-setup)