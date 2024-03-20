add_executable(Converter_FileParser converter_fileparser.cpp)
target_link_libraries(Converter_FileParser EDIE::novatel EDIE::stream_interface)
set_property(TARGET Converter_FileParser PROPERTY FOLDER "examples")
