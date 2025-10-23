# $Id$
file(
	GLOB
	EXAMPLES_GLDEMOS_SOURCES
	*.c
)
list(REMOVE_ITEM EXAMPLES_GLDEMOS_SOURCES ${CMAKE_SOURCE_DIR}/examples/gldemos/glutlayer.c)

foreach(PATH IN LISTS EXAMPLES_GLDEMOS_SOURCES)
	get_filename_component(
		TARGET
		${PATH}
		NAME_WE
	)
	add_executable(
		${TARGET} ${PATH}
	)
	target_include_directories(
		${TARGET}
		PRIVATE
		${GL_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/include
	)
	target_link_directories(
		${TARGET}
		PRIVATE
		${GL_LIBRARY_DIRS}
	)
	target_link_libraries(
		${TARGET}
		PRIVATE
		Mw
		${GL_LIBRARIES}
	)
endforeach()
