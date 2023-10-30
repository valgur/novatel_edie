file(GLOB_RECURSE NOVATEL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${NOVATEL_SOURCES})

# Creating an object file which can be used for both static and shared build without recompilation.
add_library(objnovatel OBJECT ${NOVATEL_SOURCES} $<TARGET_OBJECTS:objcommon>)
target_link_libraries(objnovatel PUBLIC EDIE::stream_interface EDIE::common_dependencies)

# Creating a static lib, with the project name, using available object file, without recompilation.
add_library(novatel $<TARGET_OBJECTS:objnovatel> $<TARGET_OBJECTS:objcommon>)
add_library(EDIE::novatel ALIAS novatel)
target_link_libraries(novatel PUBLIC EDIE::common_dependencies)
set_property(TARGET novatel PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)

foreach(target objnovatel novatel)
    target_include_directories(${target}
        PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
            "$<BUILD_INTERFACE:$<TARGET_PROPERTY:objcommon,INCLUDE_DIRECTORIES>>"
            "$<BUILD_INTERFACE:$<TARGET_PROPERTY:objstreaminterface,INCLUDE_DIRECTORIES>>"
        PRIVATE include/novatel/edie/decoders
    )
    set_property(TARGET ${target} PROPERTY FOLDER "decoders")
endforeach()

install(TARGETS novatel EXPORT EDIE-targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
