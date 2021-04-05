cmake_minimum_required(VERSION 3.19.3)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)

project(game_of_life
    VERSION 1.0
    DESCRIPTION "A simple Game of Life"
    LANGUAGES CXX
)

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}
    CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo"
    FORCE)

set(CMAKE_CXX_FLAGS_DEBUG
    "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1 -Wall -Wextra"
    CACHE STRING "Flags used by the C++ compiler during AddressSanitizer builds."
    FORCE)

set(CMAKE_CXX_FLAGS_RELEASE
    "-O3 -Wall"
    CACHE STRING "Flags used by the C++ compiler during Release builds."
    FORCE)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "-O3 -Wall -Wextra -g"
    CACHE STRING "Flags used by the C++ compiler during RelWithDebInfo builds."
    FORCE)

find_package(SFML COMPONENTS graphics window system REQUIRED)

set(sources
    ./src/rule.cc
)

add_executable(gol
    "${sources}"
    ./app/gol.cc
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(gol PRIVATE /W4)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_options(gol PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(gol PUBLIC ./include)
target_link_libraries(gol sfml-graphics sfml-window sfml-system)
