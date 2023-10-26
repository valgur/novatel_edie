add_executable(Converter_Components converter_components.cpp)
target_link_libraries(Converter_Components EDIE::novatel EDIE::stream_interface)
set_property(TARGET Converter_Components PROPERTY FOLDER "examples")
