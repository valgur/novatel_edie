set(TARGET_NAME "common")
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${TARGET_NAME} STATIC ${SOURCES})
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "decoders")

target_include_directories(${TARGET_NAME}
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/edie/decoders/common>"
)

find_package(nlohmann_json REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(spdlog_setup REQUIRED CONFIG)

target_link_libraries(${TARGET_NAME} PUBLIC
    nlohmann_json::nlohmann_json
    spdlog_setup::spdlog_setup
    $<TARGET_NAME_IF_EXISTS:spdlog::spdlog>
    $<TARGET_NAME_IF_EXISTS:spdlog::spdlog_header_only>
)

install(TARGETS ${TARGET_NAME} EXPORT EDIE-targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
