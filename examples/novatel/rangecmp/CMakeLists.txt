add_executable(range_decompressor range_decompressor_example.cpp)
target_link_libraries(range_decompressor EDIE::novatel EDIE::stream_interface)
set_property(TARGET range_decompressor PROPERTY FOLDER "examples")
