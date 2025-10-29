# $Id$
file(
	GLOB
	EXAMPLES_SOURCES
	*.c
)
list(REMOVE_ITEM EXAMPLES_SOURCES ${CMAKE_SOURCE_DIR}/examples/harvard.c)

foreach(PATH IN LISTS EXAMPLES_SOURCES)
	get_filename_component(TARGET ${PATH} NAME_WE)
	add_executable(${TARGET} ${PATH})
	target_link_libraries(
		${TARGET}
		PRIVATE
		Mw
	)
	if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
		target_link_libraries(
			${TARGET}
			PRIVATE
			m
		)
	endif()
endforeach()

add_subdirectory(basic)
