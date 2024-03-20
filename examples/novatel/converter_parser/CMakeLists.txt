add_executable(Converter_Parser converter_parser.cpp)
target_link_libraries(Converter_Parser EDIE::novatel EDIE::stream_interface)
set_property(TARGET Converter_Parser PROPERTY FOLDER "examples")
