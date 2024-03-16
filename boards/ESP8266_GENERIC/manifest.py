# base modules
include("$(PORT_DIR)/boards/manifest.py")
freeze("../../modules", "dateHandling.py")
freeze("../../modules", "datetime.py")
freeze("../../modules", "time.py")
freeze("../../modules", "max7219.py")
freeze("../../modules", "mpy_env.py")
freeze("../../modules", "msgpack.py")
freeze("../../modules", "uoauth2.py")
freeze("../../modules", "ics")


# asyncio
include("$(MPY_DIR)/extmod/asyncio")

# drivers
require("ssd1306")

# micropython-lib: file utilities
require("upysh")

# micropython-lib: umqtt
require("umqtt.simple")
require("umqtt.robust")

require("urequests")
require("ssl")
require("ics")