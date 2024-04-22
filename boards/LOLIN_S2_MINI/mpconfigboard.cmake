# Define the chip variant.
set(IDF_TARGET esp32s2)

set(SDKCONFIG_DEFAULTS
    ${MICROPY_PORT_DIR}/boards/sdkconfig.base
    ${MICROPY_PORT_DIR}/boards/sdkconfig.spiram_sx
    ${MICROPY_PORT_DIR}/boards/sdkconfig.usb
)

# Set the user C modules to include in the build.
set(USER_C_MODULES
    ${PROJECT_DIR}/src/cmodules/cexample/micropython.cmake
    ${PROJECT_DIR}/src/cmodules/cexample2/micropython.cmake
    ${PROJECT_DIR}/src/cmodules/ics/micropython.cmake
)

set(MICROPY_FROZEN_MANIFEST ${MICROPY_BOARD_DIR}/manifest.py)
