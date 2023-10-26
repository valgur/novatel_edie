file(GLOB_RECURSE HWINTERFACE_DYNAMIC_LIBRARY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HWINTERFACE_DYNAMIC_LIBRARY_SOURCES})

#Creating a shared/dynamic lib, with the project name, using available object files, without recompilation.
add_library(hwinterface_dynamic_library SHARED
    ${HWINTERFACE_DYNAMIC_LIBRARY_SOURCES}
    $<TARGET_OBJECTS:objcommon>
    $<TARGET_OBJECTS:objstreaminterface>
)
add_library(EDIE::hwinterface_dynamic_library ALIAS hwinterface_dynamic_library)
target_link_libraries(hwinterface_dynamic_library PUBLIC EDIE::common_dependencies)

# FIXME: symbols from novatel and stream_interface classes are currently not exported correctly
set_property(TARGET hwinterface_dynamic_library PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)

set_property(TARGET hwinterface_dynamic_library PROPERTY FOLDER "hw_interface")

if (WINDOWS)
    add_compile_definitions(HWINTERFACE_EXPORTS)
else()
    add_compile_definitions(HWINTERFACE_STATIC_DEFINE)
endif()

target_include_directories(hwinterface_dynamic_library
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:objcommon,INCLUDE_DIRECTORIES>>"
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:objstreaminterface,INCLUDE_DIRECTORIES>>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE include/hw_interface/dynamic_library
)

install(TARGETS hwinterface_dynamic_library
    EXPORT EDIE-targets
    COMPONENT dynamic_libs
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
