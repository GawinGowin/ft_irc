cmake_minimum_required(VERSION 3.14)
project(ft_irc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -g3 -fsanitize=address")
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -g3 -fsanitize=address")

## The cube3D sources
# file(GLOB SOURCES "cmd/cub3D/**/*.c" "cmd/cub3D/*.c")　# TODO: ファイル構成をcub3Dのときから変更
# list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/cmd/cub3D/main.c") # TODO: ファイル構成をcub3Dのときから変更

## The libft sources
# file(GLOB LIBFT_SOURCES "${CMAKE_SOURCE_DIR}/pkg/libft/*.c") # TODO: ファイル構成をcub3Dのときから変更

## Create the library
add_library(testftirc ${SOURCES} ${LIBFT_SOURCES}) 

## Link with necessary libraries
# set(LIBRARIES
#   ${CMAKE_SOURCE_DIR}/lib/libmlx_Linux.a # TODO: ファイル構成をcub3Dのときから変更
# )
target_link_libraries(testftirc ${LIBRARIES})

# target_include_directories(testftirc
#   PUBLIC ${CMAKE_SOURCE_DIR}/include) # TODO: ファイル構成をcub3Dのときから変更

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
add_subdirectory(tests)