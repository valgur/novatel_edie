set(TARGET_NAME "stream_interface")
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${TARGET_NAME} STATIC ${SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE common)
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "hw_interface")
set_property(TARGET ${TARGET_NAME} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_include_directories(${TARGET_NAME}
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE include/edie/hw_interface/stream_interface
)

install(TARGETS ${TARGET_NAME} EXPORT EDIE-targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
