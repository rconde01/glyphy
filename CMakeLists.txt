cmake_minimum_required(VERSION 2.8)

include(CheckIncludeFiles)
include(CheckIncludeFileCXX)
include(CheckSymbolExists)
include(CheckFunctionExists)

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
	set(DEFAULT_FONT "C:\\\\Windows\\\\Fonts\\\\Arial.ttf")
else(WIN32)
	execute_process(
		COMMAND bash -c "fc-match -v 2>/dev/null | grep 'file:' | cut -d'\"' -f2"
		OUTPUT_VARIABLE DEFAULT_FONT)
	string(REPLACE "\n" "" DEFAULT_FONT ${DEFAULT_FONT})
endif(WIN32)

if(WIN32)
	add_definitions(-D_USE_MATH_DEFINES)   # so M_PI etc. are available in visual studio
	add_definitions(-DNOMINMAX)            # so std::min/max work in visual studio
endif(WIN32)

add_definitions(-DHAVE_CONFIG_H)
check_include_files(time.h HAVE_TIME_H)
check_include_files(sys/time.h HAVE_SYS_TIME_H)

check_include_file_cxx(hash_map HAVE_HASH_MAP_H)
check_include_file_cxx(ext/hash_map HAVE_EXT_HASH_MAP_H)

check_symbol_exists(INFINITY math.h HAVE_INFINITY)


#test for math functions

if(GCC)
	set(CMAKE_REQUIRED_LIBRARIES m)
endif(GCC)

check_function_exists(_finite HAVE_FINITE)
check_function_exists(std::isfinite HAVE_STD_ISFINITE)

check_function_exists(std::isinf HAVE_STD_ISINF)

check_symbol_exists(isnan math.h HAVE_ISNAN)
check_function_exists(std::isnan HAVE_STD_ISNAN)
check_function_exists(_isnan HAVE_ISNAN_2)

check_function_exists(lround HAVE_LROUND)
check_function_exists(std::lround HAVE_STD_LROUND)

check_function_exists(round HAVE_ROUND)
check_function_exists(std::round HAVE_STD_ROUND)

check_function_exists(log2 HAVE_LOG2)
check_function_exists(std::log2 HAVE_STD_LOG2)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.h ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/demo")
