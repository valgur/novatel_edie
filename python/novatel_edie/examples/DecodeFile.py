from pathlib import Path

from novatel_edie import hw_interface, novatel

script_dir = Path(__file__).parent
data_path = script_dir / 'resources/bestpos.bin'
print(data_path)

# Read from a file using InputFileStream() as decoder input
in_file = hw_interface.InputFileStream(str(data_path))
decoder = novatel.Decoder(in_file)
for log in decoder:
    print(f'{log.header["MessageTime"]} {log.header["MessageName"]} - {log.body["eMyPositionStatus"]} '
          f'{log.body["dMyLatitude"]} {log.body["dMyLongitude"]} {log.body["fMyLatStdDev"]} {log.body["fMyLongStdDev"]} '
          f'{log.body["ucMyNumInSolution"]}')
in_file.close()

print('------------------')

# Read from a file using a Python binary stream as decoder input
with data_path.open('rb') as f:
    decoder = novatel.Decoder(f)
    for log in decoder:
        print(f'{log.header["MessageTime"]} {log.header["MessageName"]} - {log.body["eMyPositionStatus"]} '
              f'{log.body["dMyLatitude"]} {log.body["dMyLongitude"]} {log.body["fMyLatStdDev"]} {log.body["fMyLongStdDev"]} '
              f'{log.body["ucMyNumInSolution"]}')
