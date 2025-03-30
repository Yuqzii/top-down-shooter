cmake_minimum_required(VERSION 3.22)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

project("game" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# Change this for release
set(CMAKE_BUILD_TYPE DEBUG)

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
"src/game/UI/widget.cpp"
"src/game/UI/background.cpp"
"src/game/UI/UIManager.cpp"
"src/game/UI/slider.cpp"
"src/game/vector2D.cpp"
)

# file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/")

target_link_libraries(${PROJECT_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/libraries/libSDL2-2.0.so"
	"${CMAKE_CURRENT_SOURCE_DIR}/libraries/libSDL2_image-2.0.so"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

option(DEBUG "Debug" OFF)

if(DEBUG)
	target_compile_options(${PROJECT_NAME} PRIVATE "-D DEBUG")
endif()

# Remove when changing to static link SDL
option(MAC "Enable when building on Mac" OFF)
if (MAC)
	target_include_directories(${PROJECT_NAME} PUBLIC "/opt/homebrew/Cellar/sdl2_image/2.8.8/include")
endif()
