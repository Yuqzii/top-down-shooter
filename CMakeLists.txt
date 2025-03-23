cmake_minimum_required(VERSION 3.22)
project(game LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# Comment this for prod
set(CMAKE_BUILD_TYPE Debug)

SET(SOURCE_FILES
"src/main.cpp"
"src/game/game.cpp"
"src/game/renderManager.cpp"
"src/game/gameObject.cpp"
"src/player.cpp"
"src/bullet.cpp"
"src/enemy.cpp"
"src/game/collision.cpp"
"src/enemySpawner.cpp"
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES} SDL2_image::SDL2_image)
target_include_directories(${PROJECT_NAME} 
	PRIVATE include
	PUBLIC ${SDL2_INCLUDE_DIRS}
	PUBLIC ${SDL2IMAGE_INCLUDE_DIRS}
)

configure_file(
	${CMAKE_SOURCE_DIR}/startScript.sh.in
	${CMAKE_BINARY_DIR}/startScript.sh @ONLY
)
