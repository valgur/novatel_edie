set(TARGET_NAME "common")
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${TARGET_NAME} STATIC ${SOURCES})
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/api/ ${CMAKE_SOURCE_DIR}/bin/api/decoders/${TARGET_NAME}/api/)
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "decoders")
target_include_directories(${TARGET_NAME} PUBLIC api)

target_include_directories(${TARGET_NAME}
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/edie/decoders/common>"
)

if(BUILD_SHARED_LIBS)
    # Statically linking against spdlog causes its singleton registry to be
    # re-instantiated in each shared library and executable that links against it.
    list(APPEND CONAN_EXTRA_INSTALL_ARGS
        -o spdlog/*:shared=True -o fmt/*:shared=True
    )
endif()

find_package(nlohmann_json REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(spdlog_setup REQUIRED CONFIG)

add_library(${TARGET_NAME} INTERFACE)
target_link_libraries(${TARGET_NAME} PUBLIC
    nlohmann_json::nlohmann_json
    spdlog_setup::spdlog_setup
)
if(TARGET spdlog::spdlog)
    target_link_libraries(${TARGET_NAME} INTERFACE spdlog::spdlog)
else()
    target_link_libraries(${TARGET_NAME} INTERFACE spdlog::spdlog_header_only)
endif()

install(TARGETS ${TARGET_NAME} EXPORT EDIE-targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
