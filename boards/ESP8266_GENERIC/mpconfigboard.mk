ifeq ($(BOARD_VARIANT),)
LD_FILES = boards/esp8266_2MiB.ld

MICROPY_ESPNOW ?= 1
MICROPY_PY_BTREE ?= 1
MICROPY_VFS_FAT ?= 1
MICROPY_VFS_LFS2 ?= 1

# Add asyncio and extra micropython-lib packages (in addition to the port manifest).
FROZEN_MANIFEST ?= $(BOARD_DIR)/manifest.py

# Configure mpconfigboard.h.
CFLAGS += -DMICROPY_ESP8266_2M
endif

ifeq ($(BOARD_VARIANT),FLASH_1M)
LD_FILES = boards/esp8266_1MiB.ld

MICROPY_ESPNOW ?= 1
MICROPY_PY_BTREE ?= 1
MICROPY_VFS_LFS2 ?= 1

# Note: Implicitly uses the port manifest.

# Configure mpconfigboard.h.
CFLAGS += -DMICROPY_ESP8266_1M
endif