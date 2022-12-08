from pathlib import Path

from novatel_edie import hw_interface

script_dir = Path(__file__).parent
in_file = hw_interface.InputFileStream(str(script_dir / "resources/bestpos.asc"))

status, data = in_file.read()
print(data)
