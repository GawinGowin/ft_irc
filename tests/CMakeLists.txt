## ${CMAKE_SOURCE_DIR} == ${WORKSPACE_DIR}
include_directories(${CMAKE_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include(GoogleTest)

## Each test consists of a test executable and a test library
### Add test executables
# add_executable(validation_test # test name
# ... # sources from the test directory
# )
### Add test libraries
# target_link_libraries(validation_test testftirc gtest gtest_main)
# gtest_add_tests(TARGET validation_test)
# ...
