# dot-matrix-calendar

Displaying a marquee list of events from a google calendar on a Wemos D1 mini and a max7219-driven dot matrix led using Micropython

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