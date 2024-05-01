add_executable(Converter_FileParser converter_file_parser.cpp)
target_link_libraries(Converter_FileParser edie::novatel edie::stream_interface)
set_property(TARGET Converter_FileParser PROPERTY FOLDER "examples")
