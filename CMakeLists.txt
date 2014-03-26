cmake_minimum_required(VERSION 2.8)

project(glyphy)

SET(CMAKE_MODULE_PATH
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules"
	"${CMAKE_MODULE_PATH}")

find_package(Freetype REQUIRED)
set(HAVE_FREETYPE2 1)
find_package(OpenGL REQUIRED)
set(HAVE_GL 1)
find_package(GLUT REQUIRED)
set(HAVE_GLUT 1)
find_package(GLEW REQUIRED)
set(HAVE_GLEW 1)

if(WIN32)
	set(DEFAULT_FONT "C:\\Windows\\Fonts\\Arial.ttf")
else(WIN32)
	execute_process(
		COMMAND bash -c "fc-match -v 2>/dev/null | grep 'file:' | cut -d'\"' -f2"
		OUTPUT_VARIABLE DEFAULT_FONT)
	string(REPLACE "\n" "" DEFAULT_FONT ${DEFAULT_FONT})
endif(WIN32)

add_definitions(-DHAVE_CONFIG_H)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.h ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/demo")
