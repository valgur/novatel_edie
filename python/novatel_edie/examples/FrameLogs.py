from pathlib import Path

from novatel_edie import hw_interface, novatel

script_dir = Path(__file__).parent
input_file = hw_interface.InputFileStream(str(script_dir / "resources/bestpos.bin"))
framer = novatel.Framer(input_file)

for log in framer:
    print(log.get_data_str())
    print(bytes(log.log_data[:log.length]))
