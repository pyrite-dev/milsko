# $Id$
file(
	GLOB
	EXAMPLE_BASIC_SOURCES
	*.c
)

foreach(PATH IN LISTS EXAMPLE_BASIC_SOURCES)
	get_filename_component(
		TARGET
		${PATH}
		NAME_WE
	)
	add_executable(
		${TARGET} ${PATH}
	)
	target_link_libraries(
		${TARGET}
		PRIVATE
		Mw
	)
endforeach()
