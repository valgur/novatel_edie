file(GLOB_RECURSE COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)

# Creating an object file which can be used for both static and shared build without recompilation.
add_library(objcommon OBJECT ${COMMON_SOURCES})

set_property(TARGET objcommon PROPERTY FOLDER "decoders")

target_include_directories(objcommon
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/edie/decoders/common>"
)

find_package(nlohmann_json REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(spdlog_setup REQUIRED CONFIG)

add_library(common_dependencies INTERFACE)
add_library(EDIE::common_dependencies ALIAS common_dependencies)
target_link_libraries(common_dependencies INTERFACE
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    spdlog_setup::spdlog_setup
)

target_link_libraries(objcommon PUBLIC EDIE::common_dependencies)

install(TARGETS common_dependencies EXPORT EDIE-targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
