add_executable(Converter_CommandEncoding command_encoding.cpp)
target_link_libraries(Converter_CommandEncoding edie::novatel edie::stream_interface)
set_property(TARGET Converter_CommandEncoding PROPERTY FOLDER "examples")
