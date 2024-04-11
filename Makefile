# Makefile for custom MicroPython esp8266 board.

################################################################################
# Define your settings here.

# The board name.
BOARD ?= ESP32_GENERIC

# Location of MicroPython repository.
MICROPY_TOP ?= $(abspath lib/micropython)

# FROZEN_MANIFEST?=manifest.py

PROJECT_TOP?=$(abspath .)

################################################################################
# Define your targets here.

all: firmware

################################################################################
# Items below this line do not generally need to be changed.

BOARD_DIR = $(abspath $(MICROPY_TOP)/ports/esp32/boards/$(BOARD))
BUILD = $(abspath build)
FWBIN = $(BUILD)/firmware.bin
PORT ?= /dev/cu.usbserial-0001
BAUD ?= 460800
FLASH_MODE ?= keep
FLASH_SIZE ?= detect
PYTHON ?= python3
USER_C_MODULES ?= $(abspath cmodules/micropython.cmake)
# USER_C_MODULES ?= $(abspath lib/micropython/examples/usercmodule/cexample/micropython.cmake)

include $(MICROPY_TOP)/py/mkenv.mk
include $(MICROPY_TOP)/py/mkrules.mk

firmware:
	$(Q)$(MAKE) -j -C $(MICROPY_TOP)/ports/esp32 \
		PROJECT_TOP=$(abspath .) \
		BOARD=$(BOARD) \
		BOARD_DIR=$(BOARD_DIR) \
		BUILD=$(BUILD) \
		USER_C_MODULES=$(USER_C_MODULES)

deploy: $(FWBIN)
	$(ECHO) "Writing $< to the board"
	$(Q)esptool.py --chip esp32 --port $(PORT) --baud $(BAUD) write_flash --verify --flash_size=$(FLASH_SIZE) --flash_mode=$(FLASH_MODE) -z 0x1000 $<

erase:
	$(ECHO) "Erase flash"
	$(Q)esptool.py --port $(PORT) --baud $(BAUD) erase_flash

reset:
	echo -e "\r\nimport machine; machine.reset()\r\n" >$(PORT)
