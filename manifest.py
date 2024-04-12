# base modules
include("$(PORT_DIR)/boards/manifest.py")
freeze("$(PROJECT_TOP)/modules", "dateHandling.py")
freeze("$(PROJECT_TOP)/modules", "datetime.py")
freeze("$(PROJECT_TOP)/modules", "time.py")
freeze("$(PROJECT_TOP)/modules", "mpy_env.py")
freeze("$(PROJECT_TOP)/modules", "msgpack.py")
freeze("$(PROJECT_TOP)/modules", "uuid.py")

require("contextlib")
require("warnings")
# require("collections")

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

# package("ics", base_path="../../modules")