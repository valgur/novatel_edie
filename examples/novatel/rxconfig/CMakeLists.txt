add_executable(rxconfig rxconfig_example.cpp)
target_link_libraries(rxconfig EDIE::novatel EDIE::stream_interface)
set_property(TARGET rxconfig PROPERTY FOLDER "examples")
