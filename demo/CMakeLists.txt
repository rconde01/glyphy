cmake_minimum_required(VERSION 2.8)

project(glyphy-demo)

include_directories(
	"${CMAKE_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_SOURCE_DIR}/src"
	${GLUT_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS})

link_directories(
	"${CMAKE_SOURCE_DIR}/src")

set(SHADERS
	demo-atlas.glsl
	demo-fshader.glsl
	demo-vshader.glsl)
	
set(SHADERHEADERS)
foreach(_shaderFile ${SHADERS})
	string(REPLACE ".glsl" "-glsl.h" _sheader ${_shaderFile})
	list(APPEND SHADERHEADERS ${_sheader})
	add_custom_command(
		OUTPUT ${_sheader}
		COMMAND ${CMAKE_COMMAND} -DHEADER_FILE="${_sheader}" -DSHADER_FILE="${_shaderFile}" -P "${CMAKE_SOURCE_DIR}/src/stringize.cmake"
		DEPENDS ${_shaderFile})
endforeach()

set(GLYPHY_DEMO_SRC
	demo-atlas.h
	demo-atlas.cc
	demo-buffer.h
	demo-buffer.cc
	demo-common.h
	demo-font.h
	demo-font.cc
	demo-glstate.h
	demo-glstate.cc
	demo-shader.h
	demo-shader.cc
	demo-view.h
	demo-view.cc
	glyphy-demo.cc
	jabberwocky.h
	matrix4x4.h
	matrix4x4.c
	trackball.h
	trackball.c
	${SHADERHEADERS})
	
add_executable(glyphy-demo ${GLYPHY_DEMO_SRC})

target_link_libraries(glyphy-demo ${GLEW_LIBRARY})
target_link_libraries(glyphy-demo ${OPENGL_LIBRARY})
target_link_libraries(glyphy-demo ${GLUT_LIBRARY})
target_link_libraries(glyphy-demo ${FREETYPE_LIBRARY})

set(GLYPHY_VALIDATE_SRC
	glyphy-validate.cc)

add_executable(glyphy-validate ${GLYPHY_VALIDATE_SRC})

target_link_libraries(glyphy-validate ${GLEW_LIBRARY})
target_link_libraries(glyphy-validate ${OPENGL_LIBRARY})
target_link_libraries(glyphy-validate ${GLUT_LIBRARY})
target_link_libraries(glyphy-validate ${FREETYPE_LIBRARY})
