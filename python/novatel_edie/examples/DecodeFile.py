from pathlib import Path

from novatel_edie import hw_interface, novatel

script_dir = Path(__file__).parent
print(script_dir / "resources/bestpos.bin")
in_file = hw_interface.InputFileStream(str(script_dir / "resources/bestpos.bin"))
decoder = novatel.Decoder(in_file)

for log in decoder:
    print(f'{log.header["MessageTime"]} {log.header["MessageName"]} - {log.body["eMyPositionStatus"]} '
          f'{log.body["dMyLatitude"]} {log.body["dMyLongitude"]} {log.body["fMyLatStdDev"]} {log.body["fMyLongStdDev"]} '
          f'{log.body["ucMyNumInSolution"]}')
