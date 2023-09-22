file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)
set(STREAM_SOURCES)
list(APPEND STREAM_SOURCES ${SOURCES})

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${NOVATEL_SOURCES})

#Creating an object file which can be used for both static and shared build without recompilation.
add_library(objstreaminterface OBJECT ${STREAM_SOURCES})
add_library(obj::stream_interface ALIAS objstreaminterface)
target_link_libraries(objstreaminterface PUBLIC obj::common)

#Creating a static lib, with the project name, using available object file, without recompilation.
add_library(stream_interface $<TARGET_OBJECTS:objstreaminterface>)
add_library(edie::stream_interface ALIAS stream_interface)
target_link_libraries(stream_interface PUBLIC obj::common)
set_property(TARGET stream_interface PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)

set_property(TARGET stream_interface PROPERTY FOLDER "hw_interface")
set_property(TARGET objstreaminterface PROPERTY FOLDER "hw_interface")

target_include_directories(objstreaminterface
    PUBLIC include
    PRIVATE include/hw_interface/stream_interface
)
get_target_property(common_INCLUDE_DIRS obj::common INCLUDE_DIRECTORIES)
target_include_directories(stream_interface PUBLIC include ${common_INCLUDE_DIRS})

install(TARGETS stream_interface)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
