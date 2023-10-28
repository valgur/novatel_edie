file(GLOB COMMONTEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)

find_package(GTest REQUIRED CONFIG)

add_executable(CommonTest ${COMMONTEST_SOURCES} $<TARGET_OBJECTS:objcommon>)
set_property(TARGET CommonTest PROPERTY FOLDER "common/tests")
target_link_libraries(CommonTest PUBLIC GTest::gtest GTest::gtest_main)
target_include_directories(CommonTest PRIVATE "$<TARGET_PROPERTY:objnovatel,INCLUDE_DIRECTORIES>")

add_test(NAME CommonTest COMMAND CommonTest)
