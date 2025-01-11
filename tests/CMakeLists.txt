include(GoogleTest)

# ..:: SampleTest ::..
add_executable(SampleTest 
 ${CMAKE_CURRENT_SOURCE_DIR}/pkg/test_sample.cpp
)
target_link_libraries(SampleTest testftirc gtest_main)
target_include_directories(SampleTest PRIVATE 
  ${CMAKE_SOURCE_DIR}/pkg/application/
  ${gtest_SOURCE_DIR}
)

# ..:: GoogleTest ::..
gtest_add_tests(TARGET SampleTest)