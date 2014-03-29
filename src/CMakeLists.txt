cmake_minimum_required(VERSION 2.8)

project(glyphy)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR})

set(SHADERS
	glyphy-common.glsl
	glyphy-sdf.glsl)
	
set(SHADERHEADERS)
foreach(_shaderFile ${SHADERS})
	string(REPLACE ".glsl" "-glsl.h" _sheader ${_shaderFile})
	list(APPEND SHADERHEADERS ${_sheader})
	add_custom_command(
		OUTPUT ${_sheader}
		COMMAND ${CMAKE_COMMAND} -DHEADER_FILE="${_sheader}" -DSHADER_FILE="${_shaderFile}" -P stringize.cmake
		DEPENDS ${_shaderFile})
endforeach()


set(PUBLICHEADERS
	glyphy.h
	glyphy-freetype.h)

set(LIBGLYPHY_SRC
	glyphy-arc.cc
	glyphy-arc-bezier.hh
	glyphy-arcs.cc
	glyphy-arcs-bezier.hh
	glyphy-blob.cc
	glyphy-common.hh
	glyphy-extents.cc
	glyphy-geometry.hh
	glyphy-outline.cc
	glyphy-sdf.cc
	glyphy-shaders.cc
	${PUBLICHEADERS}
	${SHADERHEADERS})

add_library(glyphy SHARED ${LIBGLYPHY_SRC})
install(TARGETS glyphy
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
