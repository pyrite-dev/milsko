# $Id$
cmake_minimum_required(VERSION 3.25)
project(
	milsko
	VERSION 0.0
	DESCRIPTION "Milsko GUI Toolkit"
)

option(OPENGL "Compile OpenGL widget or not" ON)
if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
	option(VULKAN "Compile Vulkan widget or not" OFF)
else()
	option(VULKAN "Compile Vulkan widget or not" ON)
endif()
option(CLASSIC "Use classic theme" OFF)
option(USE_STB_IMAGE "Use stb_image" ON)
option(USE_STB_TRUETYPE "Use stb_truetype" ON)

file(
	GLOB
	SOURCES
	src/*.c src/cursor/*.c src/icon/*.c src/text/*.c src/widget/*.c external/*.c
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

target_include_directories(
	Mw
	PRIVATE
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
	if(CMAKE_SYSTEM_NAME STREQUAL "NetBSD" AND VULKAN)
		list(APPEND INCLUDE_DIRS /usr/pkg/include)
	endif()
endif()

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

add_subdirectory(examples)
