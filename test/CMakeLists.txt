CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

include(GoogleTest)

# Create test executable from object library
add_executable(unit_tests
	"Tree2D_test.cpp"
)

target_include_directories(unit_tests PRIVATE
	"${CMAKE_SOURCE_DIR}/include/"
	"${CMAKE_SOURCE_DIR}/lib/include/"
)
# Link GTest and the game library
target_link_libraries(unit_tests PRIVATE 
	GTest::gtest_main
	"${CMAKE_SOURCE_DIR}/lib/libSDL2-2.0.so.0"
	"${PROJECT_NAME}_lib"
)

# Let GTest find the tests
gtest_discover_tests(unit_tests)
