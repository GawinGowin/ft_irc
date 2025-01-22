include(GoogleTest)

# ..:: SampleTest ::..
add_executable(SampleTest 
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/test_sample.cpp
)
target_link_libraries(SampleTest testftirc gtest_main)
target_include_directories(SampleTest PRIVATE 
  ${CMAKE_SOURCE_DIR}/pkg
  ${gtest_SOURCE_DIR}
)

#..:: UsecasesTest ::..
add_executable(UsecasesTest 
 ${CMAKE_CURRENT_SOURCE_DIR}/application/useCases/test_main.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/application/useCases/test_StartServerUseCase.cpp
)
target_link_libraries(UsecasesTest testftirc gtest_main)
target_include_directories(UsecasesTest PRIVATE 
  ${CMAKE_SOURCE_DIR}/pkg
  ${gtest_SOURCE_DIR}
)

# ..:: PresentationTest ::..
# add_executable(EntrypointTest 
#  ${CMAKE_CURRENT_SOURCE_DIR}/presentation/test_main.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/presentation/test_entrypoint.cpp
# )
# target_link_libraries(EntrypointTest testftirc gtest_main gmock_main)
# target_include_directories(EntrypointTest PRIVATE 
#   ${CMAKE_SOURCE_DIR}/pkg
#   ${gtest_SOURCE_DIR}
# )

# ..:: GoogleTest ::..
gtest_add_tests(TARGET SampleTest)
gtest_add_tests(TARGET UsecasesTest)
# gtest_add_tests(TARGET EntrypointTest)