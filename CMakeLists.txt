cmake_minimum_required(VERSION 2.8)

INCLUDE(CheckIncludeFiles)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckFunctionExists)

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
CHECK_INCLUDE_FILES(time.h HAVE_TIME_H)
CHECK_INCLUDE_FILES(sys/time.h HAVE_SYS_TIME_H)

CHECK_INCLUDE_FILE_CXX(hash_map HAVE_HASH_MAP_H)
CHECK_INCLUDE_FILE_CXX(ext/hash_map HAVE_EXT_HASH_MAP_H)

CHECK_SYMBOL_EXISTS(INFINITY math.h HAVE_INFINITY)

CHECK_FUNCTION_EXISTS(_finite HAVE_FINITE)
CHECK_FUNCTION_EXISTS(std::isfinite HAVE_STD_ISFINITE)

CHECK_FUNCTION_EXISTS(std::isinf HAVE_STD_ISINF)

CHECK_SYMBOL_EXISTS(isnan math.h HAVE_ISNAN)
CHECK_FUNCTION_EXISTS(std::isnan HAVE_STD_ISNAN)
CHECK_FUNCTION_EXISTS(_isnan HAVE_ISNAN_2)

CHECK_FUNCTION_EXISTS(lround HAVE_LROUND)
CHECK_FUNCTION_EXISTS(std::lround HAVE_STD_LROUND)

CHECK_FUNCTION_EXISTS(round HAVE_ROUND)
CHECK_FUNCTION_EXISTS(std::round HAVE_STD_ROUND)

CHECK_FUNCTION_EXISTS(log2 HAVE_LOG2)
CHECK_FUNCTION_EXISTS(std::log2 HAVE_STD_LOG2)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.h ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/demo")
