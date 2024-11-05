cmake_minimum_required(VERSION 3.10)
project(GameProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    include(FindPkgConfig)
    pkg_check_modules(SDL2 REQUIRED sdl2)
else()
    find_package(SDL2 REQUIRED)
    find_package(OpenGL REQUIRED)
endif()

add_executable(game
    src/audio/AudioManager.cpp 
    src/core/collisionHandler.cpp  
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

target_include_directories(game PRIVATE ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

if(APPLE)
    target_link_libraries(game PRIVATE
        ${SDL2_LIBRARIES}
        "-framework OpenGL"
        "-framework Cocoa"
    )
else()
    target_link_libraries(game PRIVATE
        SDL2
        ${OPENGL_LIBRARIES}
    )
endif()
