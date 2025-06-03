cmake_minimum_required(VERSION 3.10)
project(tetris_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)


include_directories(include)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(tetris_cpp
    src/main.cpp
    src/Tetris.cpp
    src/GameUI.cpp
)

target_link_libraries(tetris_cpp ncursesw)

