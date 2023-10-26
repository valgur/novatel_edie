file(GLOB_RECURSE DECODERS_DYNAMIC_LIBRARY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${DECODERS_DYNAMIC_LIBRARY_SOURCES})

#Creating a shared/dynamic lib, with the project name, using available object files, without recompilation.
add_library(decoders_dynamic_library SHARED
    ${DECODERS_DYNAMIC_LIBRARY_SOURCES}
    $<TARGET_OBJECTS:objcommon>
    $<TARGET_OBJECTS:objnovatel>
)
add_library(EDIE::decoders_dynamic_library ALIAS decoders_dynamic_library)
target_link_libraries(decoders_dynamic_library PUBLIC EDIE::hwinterface_dynamic_library EDIE::common_dependencies)

# FIXME: symbols from novatel and stream_interface classes are currently not exported correctly
set_property(TARGET decoders_dynamic_library PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)

set_property(TARGET decoders_dynamic_library PROPERTY FOLDER "decoders")

if (WINDOWS)
    add_compile_definitions(DECODERS_EXPORTS)
else()
    add_compile_definitions(DECODERS_STATIC_DEFINE)
endif()

target_include_directories(decoders_dynamic_library
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:objcommon,INCLUDE_DIRECTORIES>>"
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:objnovatel,INCLUDE_DIRECTORIES>>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE include/decoders/dynamic_library
)

install(TARGETS decoders_dynamic_library
    EXPORT EDIE-targets
    COMPONENT dynamic_libs
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
