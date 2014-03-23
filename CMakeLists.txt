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

add_definitions(-DHAVE_CONFIG_H)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.h ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/demo")
