# include default manifest
include("$(PORT_DIR)/boards/manifest.py")

# include our own extra...
module("datetime.py", base_path="$(BOARD_DIR)/../../modules")
module("dateHandling.py", base_path="$(BOARD_DIR)/../../modules")
module("time.py", base_path="$(BOARD_DIR)/../../modules")
module("mpy_env.py", base_path="$(BOARD_DIR)/../../modules")
module("msgpack.py", base_path="$(BOARD_DIR)/../../modules")
module("Calendar.py", base_path="$(BOARD_DIR)/../../modules")
