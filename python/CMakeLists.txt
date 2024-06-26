find_package(Python 3.8
    REQUIRED COMPONENTS Interpreter Development.Module
    OPTIONAL_COMPONENTS Development.SABIModule)

if(NOT DEFINED nanobind_DIR)
    execute_process(COMMAND ${Python_EXECUTABLE} -c "import nanobind; print(nanobind.cmake_dir())"
        RESULT_VARIABLE cmd_result
        OUTPUT_VARIABLE nanobind_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT cmd_result EQUAL 0)
        unset(nanobind_DIR)
    endif()
endif()
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(python_bindings STABLE_ABI NB_STATIC LTO
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/bindings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/hw_interface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/common/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/common/json_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/common/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/common/nexcept.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/commander.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/file_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/filter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/framer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/header_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/message_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/range_decompressor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/novatel/rxconfig_handler.cpp
)

set_target_properties(python_bindings PROPERTIES OUTPUT_NAME bindings)
target_include_directories(python_bindings PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/bindings)
target_link_libraries(python_bindings PRIVATE EDIE::stream_interface EDIE::novatel)

target_compile_definitions(python_bindings PRIVATE
    VERSION_INFO=${SKBUILD_PROJECT_VERSION}
    # https://github.com/boostorg/system/issues/32#issuecomment-462912013
    HAVE_SNPRINTF
)

if(DEFINED SKBUILD_PROJECT_NAME)
    set(PYTHON_INSTALL_DIR ${SKBUILD_PROJECT_NAME})
elseif(NOT DEFINED PYTHON_INSTALL_DIR)
    set(PYTHON_INSTALL_DIR lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/site-packages/novatel_edie)
endif()

install(TARGETS python_bindings
    LIBRARY DESTINATION ${PYTHON_INSTALL_DIR}
    COMPONENT python)
install(DIRECTORY novatel_edie/
        DESTINATION ${PYTHON_INSTALL_DIR}
        COMPONENT python)
install(FILES ${CMAKE_SOURCE_DIR}/database/messages_public.json
        DESTINATION ${PYTHON_INSTALL_DIR}
        COMPONENT python)
