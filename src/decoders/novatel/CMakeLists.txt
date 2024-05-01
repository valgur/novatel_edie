
file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rangecmp/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rangecmp/*.h*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)
set(NOVATEL_SOURCES)
list(APPEND NOVATEL_SOURCES ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${NOVATEL_SOURCES})

# Creating an object file which can be used for both static and shared build without recompilation.
add_library(objnovatel OBJECT ${NOVATEL_SOURCES})
add_library(obj::novatel ALIAS objnovatel)
target_link_libraries(objnovatel PUBLIC obj::common edie::stream_interface)

# Creating a static lib, with the project name, using available object file, without recompilation.
add_library(novatel STATIC $<TARGET_OBJECTS:objnovatel>)
add_library(edie::novatel ALIAS novatel)
target_link_libraries(novatel PUBLIC obj::common)

set_property(TARGET novatel PROPERTY FOLDER "decoders")
set_property(TARGET objnovatel PROPERTY FOLDER "decoders")

target_include_directories(objnovatel
    PUBLIC include
    PRIVATE include/edie/decoders/novatel
)
get_target_property(common_INCLUDE_DIRS obj::common INCLUDE_DIRECTORIES)
target_include_directories(novatel PUBLIC include ${common_INCLUDE_DIRS})

file(GLOB_RECURSE MY_PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
set_target_properties(novatel PROPERTIES PUBLIC_HEADER "${MY_PUBLIC_HEADERS}")
install(TARGETS novatel PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/novatel/edie)
