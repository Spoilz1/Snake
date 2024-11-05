cmake_minimum_required(VERSION 3.10)
project(GameProject)
# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# MacOS specific setup
if(APPLE)
    # Set SDL2 paths explicitly for Homebrew
    set(SDL2_PATH "/opt/homebrew/Cellar/sdl2/2.30.9")
    set(SDL2_INCLUDE_DIR "${SDL2_PATH}/include")
    set(SDL2_LIBRARY "${SDL2_PATH}/lib/libSDL2.dylib")
    include_directories(
        ${SDL2_INCLUDE_DIR}
        "/opt/homebrew/include"
    )
else()
    find_package(SDL2 REQUIRED)
    find_package(OpenGL REQUIRED)
    include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
    )
endif()
# Source files
set(SOURCE_FILES 
    src/audio/AudioManager.cpp 
    src/core/collisionHandler 
    src/core/eventsHandler.cpp 
    src/core/Game.cpp 
    src/core/GameState.cpp 
    src/core/updateGame.cpp 
    src/input/inputHandler.cpp 
    src/rendering/addApple.cpp 
    src/rendering/addSnake.cpp 
    src/rendering/drawDigit.cpp 
    src/rendering/drawLetter.cpp 
    src/rendering/drawLine.cpp 
    src/rendering/drawSnakeEdges.cpp 
    src/rendering/drawSquare.cpp 
    src/rendering/Renderer.cpp 
    src/rendering/UI.cpp 
    src/utils/snakeUtils.cpp
)
# Add executable
add_executable(game ${SOURCE_FILES})
# Link libraries
if(APPLE)
    target_link_libraries(game 
        ${SDL2_LIBRARY}
        "-framework OpenGL"
        "-framework Cocoa"
    )
else()
    target_link_libraries(game
        SDL2
        ${OPENGL_LIBRARIES}
    )
endif()
