# dot-matrix-calendar

Displaying a marquee list of events from one or more ics calendar feeds on an ESP32 based Lolin S2 Mini and a max7219-driven dot matrix led using Micropython. The firmware is set up with a captive portal, enabling 
connection to a wifi network with password.
Application allows for several calendar feeds to be fetched and displayed at the same time.

## Getting started

For this project to work, you need the following:

- [Wemos Lolin S2 Mini](https://elkim.no/produkt/esp32-s2-mini-v1-0-0/)
- [8 module 8x8 dot matrix led displays with an SPI interface](https://www.aliexpress.com/item/1005006246992859.html?channel=twinner)
- 3D printed [stand](stand/README.md)

4- or more than 8 module led matrix displays will also work - just need to set up the display config for that.

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
![Soldered](soldered.jpg)

### Install tools for building and loading binary

To be able to build the firmware locally, the following tools are needed:

Install version 5.0.4 of the Espressif toolchain.

```bash
    pipx install pyelftools
    mkdir ~/esp
    cd ~/esp
    git clone https://github.com/espressif/esp-idf.git
    git -C esp-idf checkout v5.0.2
    ./esp-idf/install.sh
```

For programming and erasing the flash, use esptools.py

```bash
    pipx install esptool
```

For loading python files and running repl

```bash
    pipx install rshell
```

### Load binary

Ready built binary can be downloaded from the latest build from github, [latest release](https://github.com/stenjo/dot-matrix-calendar/releases/latest). Click `esp32-S2-binary` and download the zipped binary. Extract the file into `micropython.bin`.

Clone and extract this repo and move into the repository root:

```bash
git clone https://github.com/stenjo/dot-matrix-calendar.git
cd dot-matrix-calendar
```

Connect your esp32-S2 to your Mac via usb cable. Set the board into programming mode by holding button 0, pressing the reset button and releasing the button 0 when the board is connected.

Check what usb port the board is connected to by running the following command:

```bash
ls /dev/tty.*
```

You should get a listing of connected usb devices like:
![usb list](usb-name.png)
Make a note of the usb name - you are going to use this for the erasing a loading of the board flash.

Erase the flash specifying the port found in the list above:

```bash
esptool.py --port /dev/tty.usbmodem01 --baud 460800 erase_flash
```

You should get a result something like this:
![erase result](erasing-flash.png)

Program the board flash with the `micropython.bin` file that you either downloaded from latest release or built locally:

```bash
make deploy
```

Reset the board by pressing the reset button.
Add python files to the board by using rshell:

```bash
rshell rsync ../../src /pyboard
```

Command should generate a response like this:

```bash
>$ rshell
Connecting to /dev/cu.usbmodem1234561 (buffer-size 256)...
Trying to connect to REPL  connected
Retrieving sysname ... esp32
Testing if sys.stdin.buffer exists ... Y
Retrieving root directories ... /boot.py/
Setting time ... May 22, 2024 07:15:35
Evaluating board_name ... pyboard
Retrieving time epoch ... Jan 01, 2000
Welcome to rshell. Use Control-D (or the exit command) to exit rshell.
```

Within rshell, run the command to copy `boot.py` and `main.py` to the board

```bash
rsync src /pyboard
```

Should result in something like:

```bash
> rsync src /pyboard
Adding /pyboard/version.txt
Adding /pyboard/idle-timeout.txt
Adding /pyboard/main.py
Checking /pyboard/boot.py
dot-matrix-calendar/src/boot.py is newer than /pyboard/boot.py - copying
```

### Build binary

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
python -m freezefs modules/micropython-wifi-setup/lib modules/frozen_setup.py -ov always

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


## Credits

[MicroPython WiFi setup](https://github.com/george-hawkins/micropython-wifi-setup)