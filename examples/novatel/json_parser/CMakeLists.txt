add_executable(NovAtelJson_Parser json_parser.cpp)
target_link_libraries(NovAtelJson_Parser edie::novatel edie::stream_interface)
set_property(TARGET NovAtelJson_Parser PROPERTY FOLDER "examples")
