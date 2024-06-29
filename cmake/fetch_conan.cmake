#[=======================================================================[.rst:

fetch_conan.cmake
=================

Functions
---------

.. cmake:function:: find_or_download_conan()

    Finds a Conan executable from the system PATH or downloads it if it is not
    found. Takes an optional MIN_VERSION argument for the found Conan and
    a required DOWNLOAD_VERSION argument for the version to download.
    Output location can be specified with the DESTINATION argument.

.. cmake:function:: fetch_conan()

    Downloads the Conan package manager. The function takes two named arguments:
    VERSION and DESTINATION. VERSION specifies the version of Conan to fetch,
    and DESTINATION specifies the directory where Conan should be extracted.
    If DESTINATION is not provided, Conan is extracted to the
    ${CMAKE_BINARY_DIR}/conan directory.


#]=======================================================================]

function(find_or_download_conan)
    set(options "")
    set(oneValueArgs MIN_VERSION DOWNLOAD_VERSION DESTINATION)
    set(multiValueArgs "")
    include(CMakeParseArguments)
    cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

    message(STATUS "Searching for Conan executable version ${ARG_MIN_VERSION}")

    find_program(CONAN_EXECUTABLE conan)
    if(CONAN_EXECUTABLE AND ARG_MIN_VERSION)
        execute_process(
                COMMAND "${CONAN_EXECUTABLE}" --version
                OUTPUT_VARIABLE conan_output
                RESULT_VARIABLE conan_result
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ECHO_ERROR_VARIABLE
        )
        if(conan_result)
            message(FATAL_ERROR "Error when trying to run ${CONAN_EXECUTABLE} --version")
        endif()
        string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" conan_version ${conan_output})
        if(conan_version VERSION_LESS ARG_MIN_VERSION)
            message(STATUS "Conan version ${conan_version} found, but version ${ARG_MIN_VERSION} is required.")
            set(CONAN_EXECUTABLE "-NOTFOUND")
        endif()
    endif()

    if(CONAN_EXECUTABLE)
        set(CONAN_COMMAND "${CONAN_EXECUTABLE}" CACHE STRING "Path to the Conan executable" FORCE)
    else()
        message(STATUS "Suitable Conan executable not found, downloading version ${ARG_DOWNLOAD_VERSION}")
        fetch_conan(VERSION ${ARG_DOWNLOAD_VERSION} DESTINATION "${ARG_DESTINATION}")
    endif()
endfunction()

function(fetch_conan)
    set(options "")
    set(oneValueArgs VERSION DESTINATION)
    set(multiValueArgs "")
    include(CMakeParseArguments)
    cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

    if(NOT ARG_DESTINATION)
        set(ARG_DESTINATION "${CMAKE_BINARY_DIR}/conan")
    endif()

    # Architecture vars are not set unless a project is defined
    project(dummy NONE)
    if(WIN32)
        set(HOST_OS "windows")
        if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "AMD64")
            set(HOST_ARCH "x86_64")
        elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "X86")
            set(HOST_ARCH "i686")
        else()
            message(FATAL_ERROR "Cannot download Conan for ${CMAKE_HOST_SYSTEM_PROCESSOR} on Windows")
        endif()
    elseif(APPLE)
        set(HOST_OS "macos")
        if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
            set(HOST_ARCH "arm64")
        else()
            set(HOST_ARCH "x86_64")
        endif()
    elseif(UNIX)
        set(HOST_OS "linux")
        if(NOT CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64|amd64|x86_64|x64")
            message(FATAL_ERROR "Cannot download Conan for ${CMAKE_HOST_SYSTEM_PROCESSOR} on Linux")
        endif()
        set(HOST_ARCH "x86_64")
    else()
        message(FATAL_ERROR "Cannot download Conan for ${CMAKE_SYSTEM_NAME}")
    endif()

    set(CONAN_VERSION ${ARG_VERSION})
    set(FILE_EXT "tgz")
    if(WIN32)
        set(FILE_EXT "zip")
    endif()
    set(CONAN_FILE "conan-${CONAN_VERSION}-${HOST_OS}-${HOST_ARCH}.${FILE_EXT}")
    set(CONAN_URL "https://github.com/conan-io/conan/releases/download/${CONAN_VERSION}/${CONAN_FILE}")

    message(STATUS "Downloading Conan ${CONAN_VERSION} from ${CONAN_URL}")
    include(FetchContent)
    FetchContent_Declare(
        Conan
        URL "${CONAN_URL}"
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
        SOURCE_DIR "${ARG_DESTINATION}"
        DOWNLOAD_EXTRACT_TIMESTAMP 1
    )
    FetchContent_GetProperties(Conan)
    if(NOT Conan_POPULATED)
        FetchContent_Populate(Conan)
    endif()

    set(CONAN_COMMAND "${ARG_DESTINATION}/conan" CACHE STRING "Path to the Conan executable")
endfunction()
