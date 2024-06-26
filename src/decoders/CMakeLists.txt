add_subdirectory(common)
add_subdirectory(novatel)

if(BUILD_C_API)
    add_subdirectory(c_api)
endif()
