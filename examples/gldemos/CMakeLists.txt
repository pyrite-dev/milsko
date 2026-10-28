file(
	GLOB
	EXAMPLES_GL_SOURCES
	*.c
)
if(NOT CMAKE_SYSTEM_NAME MATCHES Retro)
	find_package(OpenGL)
endif()

foreach(PATH IN LISTS EXAMPLES_GL_SOURCES)
	get_filename_component(TARGET ${PATH} NAME_WE)
    if(${TARGET} STREQUAL "glutlayer")
	    continue()
	endif()
	if(CMAKE_SYSTEM_NAME MATCHES Retro)
		add_application(${TARGET} ${PATH} ${PROJECT_SOURCE_DIR}/resource/macos.r)
		# import stubs for OpenGLLibrary and OpenGLUtility
		target_link_libraries(
			${TARGET}
			PRIVATE
			Mw
			${PROJECT_SOURCE_DIR}/external/agl/lib/libglutil.a
			${PROJECT_SOURCE_DIR}/external/agl/lib/libgl.a
		)
	else()
		add_executable(${TARGET} MACOSX_BUNDLE ${PATH})
		target_link_libraries(
			${TARGET}
			PRIVATE
			Mw
			OpenGL::GL OpenGL::GLU
		)
	endif()
	if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
		target_link_libraries(
			${TARGET}
			PRIVATE
			m
		)
	endif()
endforeach()
