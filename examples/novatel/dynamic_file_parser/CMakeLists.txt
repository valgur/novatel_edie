add_executable(NovAtelDynamic_FileParser dynamic_file_parser.cpp)
target_link_libraries(NovAtelDynamic_FileParser EDIE::decoders_dynamic_library)
set_property(TARGET NovAtelDynamic_FileParser PROPERTY FOLDER "examples")
