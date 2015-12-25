project(Urharc)
cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++11 -Wall -Wextra)
if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "Setting build type to Release (none was specified)")
	SET(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Os")

add_executable(${PROJECT_NAME}
	controllable.cpp
	grafikus_rutinok.cpp
	laser.cpp
	mestint.cpp
	urhajo.cpp
	game_engine.cpp
	hittable.cpp
	main.cpp
	player.cpp
	tukor.cpp)

# Copy the font file to the build directory
configure_file("font.otf" "font.otf" COPYONLY)

# Link SFML
find_package(SFML 2 REQUIRED system window graphics)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
