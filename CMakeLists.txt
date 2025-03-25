cmake_minimum_required(VERSION 3.25.1)
project(ft_irc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -g3 -fsanitize=address -fprofile-arcs -ftest-coverage -O0")
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -g3 -fsanitize=address -fprofile-arcs -ftest-coverage -O0")

## Sources
file(GLOB_RECURSE SOURCES
"${CMAKE_SOURCE_DIR}/pkg/application/**/*.cpp"
"${CMAKE_SOURCE_DIR}/pkg/domain/**/*.cpp"
"${CMAKE_SOURCE_DIR}/pkg/infra/**/*.cpp"
"${CMAKE_SOURCE_DIR}/pkg/presentation/**/*.cpp"
"${CMAKE_SOURCE_DIR}/pkg/presentation/*.cpp"
)
message(STATUS "SOURCES: ${SOURCES}")
add_library(testftirc ${SOURCES}) 
add_executable(ft_irc
${CMAKE_SOURCE_DIR}/cmd/ft_irc/main.cpp)

## Sentry
find_path(SENTRY_INCLUDE_DIRS sentry.h)
find_library(SENTRY_LIBRARY NAMES sentry)

target_include_directories(
  ft_irc
  PRIVATE ${CMAKE_SOURCE_DIR}/pkg/
)
target_link_libraries(ft_irc testftirc)

if (SENTRY_INCLUDE_DIRS AND SENTRY_LIBRARY)
  add_compile_definitions(USE_SENTRY=1)
  message(STATUS "Using Sentry for logging")
  message(STATUS "include dir " ${SENTRY_INCLUDE_DIRS})
  message(STATUS "library " ${SENTRY_LIBRARY})
  target_include_directories(
    testftirc
    PRIVATE ${CMAKE_SOURCE_DIR}/pkg/
    PRIVATE ${SENTRY_INCLUDE_DIRS}
  )
  target_include_directories(
    ft_irc
    PRIVATE ${CMAKE_SOURCE_DIR}/pkg/
    PRIVATE ${SENTRY_INCLUDE_DIRS}
  )
  target_link_libraries(ft_irc testftirc ${SENTRY_LIBRARY})
  target_link_libraries(testftirc ${SENTRY_LIBRARY})
else()
  message(STATUS "Sentry not found, falling back to console logging")
  target_include_directories(
    testftirc
    PRIVATE ${CMAKE_SOURCE_DIR}/pkg/
  )
  target_include_directories(
    ft_irc
    PRIVATE ${CMAKE_SOURCE_DIR}/pkg/
  )
  target_link_libraries(ft_irc testftirc)
endif()

## install gtest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
## For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
FetchContent_GetProperties(googletest)

enable_testing()
add_subdirectory(tests/gtests)