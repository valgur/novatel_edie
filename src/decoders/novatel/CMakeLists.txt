set(TARGET_NAME "novatel")
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${TARGET_NAME} STATIC ${SOURCES})
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "decoders")
target_link_libraries(${TARGET_NAME} PUBLIC stream_interface common)
set_property(TARGET ${TARGET_NAME} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_include_directories(${TARGET_NAME}
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    PRIVATE include/edie/decoders/novatel
)
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "decoders")

install(TARGETS ${TARGET_NAME} EXPORT EDIE-targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
