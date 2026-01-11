cmake_minimum_required(VERSION 3.25)
project(
	milsko
)

include(CheckIncludeFiles)

include(GNUInstallDirs)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

option(TRY_OPENGL "Try to compile OpenGL widget or not" ON)
option(TRY_VULKAN "Try to compile Vulkan widget or not" ON)
option(CLASSIC_THEME "Use classic theme" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(USE_STB_IMAGE "Use stb_image" ON)
option(USE_STB_TRUETYPE "Use stb_truetype" OFF)
option(USE_FREETYPE2 "Use FreeType 2" ON)
option(BUILD_SHARED "Build a shared library" ON)
option(BUILD_STATIC "Build a static library" OFF)
option(INSTALL_HEADERS "Install headers" ON)

file(
	GLOB
	SOURCES
	src/*.c src/cursor/*.c src/icon/*.c src/text.c src/widget/*.c src/math/*.c src/font/*.c src/dialog/*.c src/abstract/*.c external/*.c
)

list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/widget/opengl.c")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/widget/vulkan.c")

if(NOT USE_STB_IMAGE)
	list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image.c")
endif()

if(NOT USE_STB_TRUETYPE)
	list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/stb_truetype.c")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
	list(APPEND CMAKE_REQUIRED_INCLUDES "/usr/X11R7/include")
	list(APPEND CMAKE_REQUIRED_INCLUDES "/usr/pkg/include")
endif()

if(TRY_OPENGL)
	check_include_files(GL/gl.h HAVE_GL_GL_H)
	if(HAVE_GL_GL_H)
		set(BUILD_OPENGL ON)
		list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/widget/opengl.c")
		message(STATUS "OpenGL widget has been enabled")
	else()
		message(WARNING "OpenGL widget has been disabled")
	endif()
endif()

if(TRY_OPENGL)
	check_include_files(vulkan/vulkan.h HAVE_VULKAN_VULKAN_H)
	if(HAVE_VULKAN_VULKAN_H)
		set(BUILD_VULKAN ON)
		list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/widget/vulkan.c")
		message(STATUS "Vulkan widget has been enabled")
	else()
		message(WARNING "Vulkan widget has been disabled")
	endif()
endif()

if(BUILD_STATIC)
message(STATUS "Building Milsko as a static library")
add_library(
	Mw
	${SOURCES}
)
elseif(BUILD_SHARED)
message(STATUS "Building Milsko as a shared library")
add_library(
	Mw
	SHARED
	${SOURCES}
)
else()
message(ERROR "Must either build a shared library or a static library")
endif()

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

if(CLASSIC_THEME)
	target_compile_definitions(
		Mw
		PRIVATE
		USE_CLASSIC_THEME
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
	target_link_options(
		Mw
		PRIVATE
		-static-libgcc
	)
else()
	find_package(PkgConfig)
	pkg_check_modules(X11 REQUIRED x11)
	pkg_check_modules(XRENDER REQUIRED xrender)

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
	list(APPEND INCLUDE_DIRS ${X11_INCLUDE_DIRS} ${XRENDER_INCLUDE_DIRS})
	list(APPEND LIBRARY_DIRS ${X11_LIBRARY_DIRS} ${XRENDER_LIBRARY_DIRS})
	list(APPEND LIBRARIES ${X11_LIBRARIES} ${XRENDER_LIBRARIES} m)
	if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		list(APPEND LIBRARIES dl)
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

if(BUILD_VULKAN)
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

install(
	TARGETS Mw
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(INSTALL_HEADERS)
	install(
		DIRECTORY include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		FILES_MATCHING PATTERN "*.h"
	)
endif()

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
