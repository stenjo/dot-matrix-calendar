# Makefile for custom MicroPython stm32 board.

################################################################################
# Define your settings here.

# The board name.
BOARD ?= UNIX
VARIANT ?= "coverage"

# Location of MicroPython repository.
MICROPY_TOP ?= $(abspath ../../lib/micropython)
PROJECT_TOP ?= $(abspath ../..)
USER_C_MODULES ?= $(abspath $(PROJECT_TOP)/cmodules)
# USER_C_MODULES ?= $(abspath ../../lib/micropython/examples/usercmodule)
################################################################################
# Define your targets here.

all: firmware

################################################################################
# Items below this line do not generally need to be changed.

BOARD_DIR = $(abspath .)
BUILD = $(abspath build)

include $(MICROPY_TOP)/py/mkenv.mk
include $(MICROPY_TOP)/py/mkrules.mk
include $(BOARD_DIR)/mpconfigport.mk

firmware:
	$(Q)$(MAKE) -C $(MICROPY_TOP)/ports/unix \
		PROJECT_TOP=$(abspath ../..) \
		BOARD=$(BOARD) \
		BOARD_DIR=$(BOARD_DIR) \
		BUILD=$(BUILD) \
		FROZEN_MANIFEST=$(FROZEN_MANIFEST) \
		USER_C_MODULES=$(USER_C_MODULES) \
		VARIANT=$(VARIANT) \

deploy:
	$(PYTHON) $(MICROPY_TOP)/tools/pydfu.py -u $(BUILD)/firmware.dfu
