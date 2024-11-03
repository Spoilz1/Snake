
cmake_minimum_required(VERSION 3.10)

project(GameProject)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("/opt/homebrew/include")

link_directories("/opt/homebrew/lib")

set(SOURCE_FILES src/main.cpp src/game.cpp)

add_executable(game ${SOURCE_FILES})

target_link_libraries(game SDL2)