cmake_minimum_required(VERSION 3.14)
project(ft_irc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -g3 -fsanitize=address")
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -g3 -fsanitize=address")

## Sources
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/pkg/**/*.cpp")
message(STATUS "SOURCES: ${SOURCES}")
add_library(testftirc ${SOURCES}) 

target_include_directories(
  testftirc
  PRIVATE ${CMAKE_SOURCE_DIR}/pkg/
)

## Link with necessary libraries
set(LIBRARIES pthread)
target_link_libraries(testftirc ${LIBRARIES})

## install gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
## For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
FetchContent_GetProperties(googletest)

enable_testing()
add_subdirectory(pkg/tests)