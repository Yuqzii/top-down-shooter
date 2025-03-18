cmake_minimum_required(VERSION 3.22)
project(MyProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

SET(SOURCE_FILES
"src/main.cpp"
"src/game/game.cpp"
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(game ${SOURCE_FILES})
target_link_libraries(game ${SDL2_LIBRARIES})
target_include_directories(game PRIVATE include)
