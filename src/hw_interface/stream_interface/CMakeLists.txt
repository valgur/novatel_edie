file(GLOB_RECURSE STREAM_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
)

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${STREAM_SOURCES})

#Creating an object file which can be used for both static and shared build without recompilation.
add_library(objstreaminterface OBJECT ${STREAM_SOURCES} $<TARGET_OBJECTS:objcommon>)
target_link_libraries(objstreaminterface PUBLIC EDIE::common_dependencies)

#Creating a static lib, with the project name, using available object file, without recompilation.
add_library(stream_interface $<TARGET_OBJECTS:objstreaminterface> $<TARGET_OBJECTS:objcommon>)
add_library(EDIE::stream_interface ALIAS stream_interface)
target_link_libraries(stream_interface PUBLIC EDIE::common_dependencies)
set_property(TARGET stream_interface PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)

foreach(target objstreaminterface stream_interface)
    target_include_directories(${target}
        PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
            "$<BUILD_INTERFACE:$<TARGET_PROPERTY:objcommon,INCLUDE_DIRECTORIES>>"
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        PRIVATE include/hw_interface/stream_interface
    )
    set_property(TARGET ${target} PROPERTY FOLDER "hw_interface")
endforeach()

install(TARGETS stream_interface EXPORT EDIE-targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
