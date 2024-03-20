add_executable(Converter_CommandEncoding command_encoding.cpp)
target_link_libraries(Converter_CommandEncoding EDIE::novatel EDIE::stream_interface)
set_property(TARGET Converter_CommandEncoding PROPERTY FOLDER "examples")
