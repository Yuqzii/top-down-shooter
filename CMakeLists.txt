cmake_minimum_required(VERSION 3.22)
project(MyProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

SET(SOURCE_FILES
"src/main.cpp"
"src/game/game.cpp"
"src/game/renderManager.cpp"
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(game ${SOURCE_FILES})
target_link_libraries(game PUBLIC ${SDL2_LIBRARIES} SDL2_image::SDL2_image)
target_include_directories(game 
	PRIVATE include
	PUBLIC ${SDL2_INCLUDE_DIRS}
	PUBLIC ${SDL2IMAGE_INCLUDE_DIRS}
)
