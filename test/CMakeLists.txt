CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.16.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# Create test executable from object library
add_executable(unit_tests
	"Tree2D_test.cpp"
	"collision_test.cpp"
)

target_include_directories(unit_tests PRIVATE
	"${CMAKE_SOURCE_DIR}/include/"
	"${CMAKE_SOURCE_DIR}/lib/include/"
)
# Link GTest and the game library
target_link_libraries(unit_tests PRIVATE 
	GTest::gtest_main
	"${PROJECT_NAME}_lib"
)

# Let GTest find the tests
gtest_discover_tests(unit_tests)
