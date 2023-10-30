if(DEFINED VCPKG_TOOLCHAIN)
    set(USE_CONAN_DEFAULT OFF)
else()
    set(USE_CONAN_DEFAULT ON)
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

string(FIND "${CMAKE_TOOLCHAIN_FILE}" "conan_toolchain.cmake" FOUND_CONAN_TOOLCHAIN)
if(FOUND_CONAN_TOOLCHAIN GREATER_EQUAL 0)
    set(BUILD_TRIGGERED_BY_CONAN TRUE)
endif()

option(USE_CONAN "Use Conan to automatically manage dependencies" ${USE_CONAN_DEFAULT})

set(CONAN_ENABLED FALSE)
if(USE_CONAN AND NOT BUILD_TRIGGERED_BY_CONAN)
    if(CMAKE_VERSION GREATER_EQUAL 3.24)
        set(CONAN_EXTRA_INSTALL_ARGS
            # Deploy the installed dependencies in the build dir for easier installation
            --deployer=full_deploy --deployer-folder=${CMAKE_BINARY_DIR}
        )
        list(APPEND CMAKE_PROJECT_TOP_LEVEL_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/conan_provider.cmake)
        set(CONAN_ENABLED TRUE)
    else()
        message(WARNING
            "CMake 3.24 or greater is required to install Conan dependencies automatically. "
            "You will have to run "
            "'conan install . --build=missing --deployer=full_deploy --deployer-folder=build/Release'"
            "manually in the source directory instead."
        )
        # To use the output from the Conan CMakeDeps generator
        list(PREPEND CMAKE_PREFIX_PATH
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/generators
            ${CMAKE_CURRENT_BINARY_DIR}/build/${CMAKE_BUILD_TYPE}/generators
        )
    endif()
endif()

option(INSTALL_THIRD_PARTY "Install third-party dependencies alongside the project" ${CONAN_ENABLED})

set(CONAN_DEPLOYER_DIR "${CMAKE_BINARY_DIR}/full_deploy/host")

function(install_third_party_libs)
    # Installs all include directories from Conan packages deployed to the build directory by 'conan install --deploy'
    set(configurations ${CMAKE_CONFIGURATION_TYPES})
    if(NOT configurations)
        set(configurations ${CMAKE_BUILD_TYPE})
    endif()
    file(GLOB_RECURSE conaninfo_files "${CONAN_DEPLOYER_DIR}/*/conaninfo.txt")
    foreach(conaninfo_path ${conaninfo_files})
        # Get relative path
        file(RELATIVE_PATH rel_conaninfo_path "${CONAN_DEPLOYER_DIR}" "${conaninfo_path}")
        # Get package name
        string(REGEX REPLACE "([^/]+)/.*$" "\\1" package_name "${rel_conaninfo_path}")
        # Get package root dir
        get_filename_component(package_root "${conaninfo_path}" DIRECTORY)
        # Skip build-only packages
        if(package_name STREQUAL "gtest")
            continue()
        endif()
        # Copy includes
        if(EXISTS "${package_root}/include")
            install(DIRECTORY "${package_root}/include/"
                    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/third_party")
        endif()
        foreach(config ${configurations})
            if(NOT rel_conaninfo_path MATCHES ".+/${config}/.+")
                continue()
            endif()
            # Copy .a, .so, .lib, .dylib
            file(GLOB_RECURSE lib_files "${package_root}/lib/*")
            foreach(lib ${lib_files})
                if(lib MATCHES "\\.(a|so[^/]*]|lib|dylib)$")
                    install(FILES "${lib}"
                            DESTINATION "${CMAKE_INSTALL_LIBDIR}/third_party"
                            CONFIGURATIONS ${config})
                endif()
            endforeach()
            # Copy .dll
            file(GLOB_RECURSE dll_files "${package_root}/bin/*")
            foreach(dll ${dll_files})
                if(dll MATCHES "\\.dll$")
                    install(FILES "${dll}"
                            DESTINATION "${CMAKE_INSTALL_BINDIR}/third_party"
                            CONFIGURATIONS ${config})
                endif()
            endforeach()
        endforeach()
    endforeach()
endfunction()

function(copy_third_party_dlls)
    # Copy all third-party DLLs to build/bin for tests and executables
    file(GLOB_RECURSE dll_files "${CONAN_DEPLOYER_DIR}/*/*.dll")
    if(CMAKE_CONFIGURATION_TYPES)
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            foreach(dll ${dll_files})
                file(RELATIVE_PATH dll_rel "${CONAN_DEPLOYER_DIR}" "${dll}")
                if(dll_rel MATCHES ".+/${config}/.+")
                    file(COPY ${dll} DESTINATION "${CMAKE_BINARY_DIR}/bin/${config}")
                endif()
            endforeach()
        endforeach()
    else()
        file(COPY ${dll_files} DESTINATION "${CMAKE_BINARY_DIR}/bin")
    endif()
endfunction()
