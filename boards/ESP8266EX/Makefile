# Makefile for custom MicroPython stm32 board.

################################################################################
# Define your settings here.

# The board name.
BOARD ?= ESP8266EX

# Location of MicroPython repository.
MICROPY_TOP ?= $(abspath ../../lib/micropython)

################################################################################
# Define your targets here.

all: firmware

################################################################################
# Items below this line do not generally need to be changed.

BOARD_DIR = $(abspath .)
BUILD = $(abspath build)
CFLAGS += -DESP8266

include $(MICROPY_TOP)/py/mkenv.mk
include $(MICROPY_TOP)/py/mkrules.mk

firmware:
	$(Q)$(MAKE) -C $(MICROPY_TOP)/ports/esp8266 \
		PROJECT_TOP=$(abspath ../..) \
		BOARD=$(BOARD) \
		BOARD_DIR=$(BOARD_DIR) \
		CFLAGS=$(CFLAGS) \
		BUILD=$(BUILD) 

