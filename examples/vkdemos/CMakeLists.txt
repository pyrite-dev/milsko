# $Id$
file(
	GLOB
	EXAMPLES_VKDEMOS_SOURCES
	*.c
)

foreach(PATH IN LISTS EXAMPLES_VKDEMOS_SOURCES)
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
		${VK_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/include
	)
	target_link_libraries(
		${TARGET}
		PRIVATE
		Mw
	)
endforeach()
