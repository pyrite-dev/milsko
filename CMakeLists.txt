# $Id$
cmake_minimum_required(VERSION 3.25)
project(
	milsko
	VERSION 0.0
	DESCRIPTION "Milsko GUI Toolkit"
)

include(CheckIncludeFiles)

option(OPENGL "Compile OpenGL widget or not" ON)
if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
	option(VULKAN "Compile Vulkan widget or not" OFF)
else()
	option(VULKAN "Compile Vulkan widget or not" ON)
endif()
option(CLASSIC "Use classic theme" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(USE_STB_IMAGE "Use stb_image" ON)
option(USE_STB_TRUETYPE "Use stb_truetype" OFF)
option(USE_FREETYPE2 "Use FreeType 2" ON)

file(
	GLOB
	SOURCES
	src/*.c src/cursor/*.c src/icon/*.c src/text.c src/widget/*.c src/math/*.c src/font/*.c src/dialog/*.c src/abstract/*.c external/*.c
)

if(NOT OPENGL)
	list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/widget/opengl.c")
endif()

if(NOT VULKAN)
	list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/widget/vulkan.c")
endif()

if(NOT USE_STB_IMAGE)
	list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image.c")
endif()

if(NOT USE_STB_TRUETYPE)
	list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/stb_truetype.c")
endif()

add_library(
	Mw
	SHARED
	${SOURCES}
)

if(USE_STB_IMAGE)
	target_compile_definitions(
		Mw
		PRIVATE
		USE_STB_IMAGE
	)
else()
	file(
		GLOB
		IMAGE_SOURCES
		external/libz/src/*.c external/libjpeg/src/*.c external/libpng/src/*.c
	)
	target_sources(
		Mw
		PRIVATE
		${IMAGE_SOURCES}
	)
	target_include_directories(
		Mw
		PRIVATE
		external/libz/include external/libjpeg/include external/libpng/include
	)
endif()

if(USE_STB_TRUETYPE)
	target_compile_definitions(
		Mw
		PRIVATE
		USE_STB_TRUETYPE
	)
endif()

if(USE_FREETYPE2)
	target_compile_definitions(
		Mw
		PRIVATE
		USE_FREETYPE2
	)

	find_package(PkgConfig)
	pkg_check_modules(FT2 REQUIRED freetype2)
	list(APPEND INCLUDE_DIRS ${FT2_INCLUDE_DIRS})
	list(APPEND LIBRARY_DIRS ${FT2_LIBRARY_DIRS})
	list(APPEND LIBRARIES ${FT2_LIBRARIES})
endif()

target_include_directories(
	Mw
	PUBLIC
	include
)

if(CLASSIC)
	target_compile_definitions(
		Mw
		PRIVATE
		MW_CLASSIC_THEME
	)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_sources(
		Mw
		PRIVATE
		src/backend/gdi.c
	)
	target_compile_definitions(
		Mw
		PRIVATE
		USE_GDI
	)
	list(APPEND LIBRARIES gdi32)
else()
	find_package(PkgConfig)
	pkg_check_modules(X11 REQUIRED x11)
	pkg_check_modules(XRENDER REQUIRED xrender)
	pkg_check_modules(XCURSOR REQUIRED xcursor)

	target_sources(
		Mw
		PRIVATE
		src/backend/x11.c
	)
	target_compile_definitions(
		Mw
		PRIVATE
		USE_X11
	)
	list(APPEND INCLUDE_DIRS ${X11_INCLUDE_DIRS} ${XRENDER_INCLUDE_DIRS} ${XCURSOR_INCLUDE_DIRS})
	list(APPEND LIBRARY_DIRS ${X11_LIBRARY_DIRS} ${XRENDER_LIBRARY_DIRS} ${XCURSOR_LIBRARY_DIRS})
	list(APPEND LIBRARIES ${X11_LIBRARIES} ${XRENDER_LIBRARIES} ${XCURSOR_LIBRARIES} m)
	if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		list(APPEND LIBRARIES dl)
	endif()
	if(VULKAN)
		if(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
			list(APPEND INCLUDE_DIRS /usr/pkg/include)
		else()
			pkg_check_modules(VULKAN REQUIRED vulkan)
			list(APPEND INCLUDE_DIRS ${VULKAN_INCLUDE_DIRS})
		endif()
	endif()
endif()

set_source_files_properties(
	src/math/mmx.c
	PROPERTIES
	COMPILE_FLAGS -mmmx
)

target_include_directories(
	Mw
	PRIVATE
	${INCLUDE_DIRS}
)
target_link_directories(
	Mw
	PRIVATE
	${LIBRARY_DIRS}
)
target_link_libraries(
	Mw
	PRIVATE
	${LIBRARIES}
)

if(VULKAN)
	check_include_files(vulkan/vk_enum_string_helper.h HAS_VK_ENUM_STRING_HELPER)
	if(HAS_VK_ENUM_STRING_HELPER)
		target_compile_definitions(
			Mw
			PRIVATE
			HAS_VK_ENUM_STRING_HELPER
		)
	endif()
endif()

target_compile_definitions(
	Mw
	PRIVATE
	_MILSKO
)

include(GNUInstallDirs)
install(
	TARGETS Mw
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
	DIRECTORY ${CMAKE_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
