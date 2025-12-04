file(
	GLOB
	EXAMPLES_BASIC_SOURCES
	*.c
)

foreach(PATH IN LISTS EXAMPLES_BASIC_SOURCES)
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
