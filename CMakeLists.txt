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
"src/engine/game.cpp"
"src/engine/scene.cpp"
"src/engine/resourceManager.cpp"
"src/engine/gameObject.cpp"
"src/player.cpp"
"src/bullet.cpp"
"src/enemy.cpp"
"src/engine/collision.cpp"
"src/enemyManager.cpp"
"src/engine/UI/widget.cpp"
"src/engine/UI/background.cpp"
"src/engine/renderManager.cpp"
"src/engine/UI/slider.cpp"
"src/engine/vector2D.cpp"
"src/engine/Tree2D.cpp"
)

# Build the game as a static library
add_library(${PROJECT_NAME}_lib STATIC ${SOURCE_FILES})

# Include directories for game library
target_include_directories(${PROJECT_NAME}_lib PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/include/"
)

# Link Linux libraries
if(UNIX AND NOT APPLE)
	message("Linux platform detected.")
	target_link_libraries(${PROJECT_NAME}_lib PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/lib/libSDL2-2.0.so.0"
		"${CMAKE_CURRENT_SOURCE_DIR}/lib/libSDL2_image-2.0.so.0"
		)
endif()

# Link Mac libraries
if(APPLE)
	message("Apple platform detected.")
	target_link_libraries(${PROJECT_NAME}_lib PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/lib/libSDL2-2.0.0.dylib"
		"${CMAKE_CURRENT_SOURCE_DIR}/lib/libSDL2_image-2.0.0.dylib"
	)
endif()

option(DEBUG_GIZMO "Enable this to render debug gizmos" OFF)
if(DEBUG_GIZMO)
	target_compile_options(${PROJECT_NAME}_lib PRIVATE "-D DEBUG_GIZMO")
endif()

# Define assets path
target_compile_definitions(${PROJECT_NAME}_lib PRIVATE
	ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)

# Testing
option(TESTING "Set this to on to build for tests" OFF)
if(TESTING)
	enable_testing()
	add_subdirectory(test)
else() # Not running tests, build game executable
	# Create game executable
	add_executable(${PROJECT_NAME} "src/main.cpp")

	if(DEBUG_GIZMO)
		target_compile_options(${PROJECT_NAME} PRIVATE "-D DEBUG_GIZMO")
	endif()

	# Link game library to executable
	target_link_libraries(${PROJECT_NAME} PRIVATE "${PROJECT_NAME}_lib")
	target_include_directories(${PROJECT_NAME} PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/include/"
		"${CMAKE_CURRENT_SOURCE_DIR}/lib/include/"
	)

endif()

