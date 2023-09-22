add_executable(NovAtelConverter_Parser converter_parser.cpp)
target_link_libraries(NovAtelConverter_Parser edie::novatel edie::stream_interface)
set_property(TARGET NovAtelConverter_Parser PROPERTY FOLDER "examples")
