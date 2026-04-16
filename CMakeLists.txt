cmake_minimum_required(VERSION 3.25)
project(
 milsko
)

include(CheckIncludeFiles)

include(GNUInstallDirs)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


option(MW_CLASSIC_THEME "Use classic theme" OFF)
option(MW_BUILD_EXAMPLES "Build examples" OFF)
option(MW_USE_STB_IMAGE "Use stb_image" ON)
option(MW_USE_STB_TRUETYPE "Use stb_truetype" OFF)

if(${CMAKE_SYSTEM_NAME} MATCHES Retro)
  option(MW_BUILD_SHARED "Build a shared library" OFF)
  option(MW_BUILD_STATIC "Build a static library" ON)
  option(MW_TRY_OPENGL "Try to compile OpenGL widget or not" OFF)
  option(MW_TRY_VULKAN "Try to compile Vulkan widget or not" OFF)
else()
  option(MW_BUILD_SHARED "Build a shared library" ON)
  option(MW_BUILD_STATIC "Build a static library" OFF)
  option(MW_TRY_OPENGL "Try to compile OpenGL widget or not" ON)
  option(MW_TRY_VULKAN "Try to compile Vulkan widget or not" ON)
endif()

option(MW_INSTALL_HEADERS "Install headers" ON)

if(${CMAKE_SYSTEM_NAME} STREQUAL Darwin OR ${CMAKE_SYSTEM_NAME} MATCHES Retro)
  option(MW_USE_FREETYPE2 "Use FreeType 2" OFF)
else()
  option(MW_USE_FREETYPE2 "Use FreeType 2" ON)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
  option(MW_USE_WAYLAND "Enable Wayland backend" ON)
endif()

file(
 GLOB
 SOURCES
 external/libz/src/*.c src/*.c src/cursor/*.c src/icon/*.c src/widget/*.c src/text/*.c src/text/font/*.c src/dialog/*.c src/abstract/*.c external/*.c
)

if(NOT MW_USE_STB_IMAGE)
  list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image.c")
endif()

if(NOT MW_USE_STB_TRUETYPE)
  list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/stb_truetype.c")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
  list(APPEND CMAKE_REQUIRED_INCLUDES "/usr/X11R7/include")
  list(APPEND CMAKE_REQUIRED_INCLUDES "/usr/pkg/include")
endif()

if(MW_TRY_OPENGL)
  find_package(OpenGL)
  if(OpenGL_FOUND)
    set(MW_BUILD_OPENGL ON)
    target_compile_definitions(
          Mw
          PRIVATE
          MW_OPENGL
        )
    if(APPLE)
      list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/widget/opengl_cocoa.m")
    endif()
    message(STATUS "OpenGL widget has been enabled")
  else()
    message(WARNING "OpenGL widget has been disabled")
  endif()
endif()

if(MW_TRY_VULKAN)
  check_include_files(vulkan/vulkan.h HAVE_VULKAN_VULKAN_H)
  if(HAVE_VULKAN_VULKAN_H)
    set(MW_BUILD_VULKAN ON)
    target_compile_definitions(
          Mw
          PRIVATE
          MW_VULKAN
        )
    message(STATUS "Vulkan widget has been enabled")
  else()
    message(WARNING "Vulkan widget has been disabled")
  endif()
endif()

if(MW_BUILD_STATIC)
  message(STATUS "Building Milsko as a static library")
  add_library(
 Mw
 ${SOURCES}
)
elseif(MW_BUILD_SHARED)
  message(STATUS "Building Milsko as a shared library")
  add_library(
 Mw
 SHARED
 ${SOURCES}
)
else()
  message(ERROR "Must either build a shared library or a static library")
endif()

if(MW_USE_STB_IMAGE)
  target_compile_definitions(
  Mw
  PRIVATE
  USE_STB_IMAGE
 )
else()
  file(
  GLOB
  IMAGE_SOURCES
  external/libjpeg/src/*.c external/libpng/src/*.c
 )
  target_sources(
  Mw
  PRIVATE
  ${MW_IMAGE_SOURCES}
 )
  target_include_directories(
  Mw
  PRIVATE
  external/libjpeg/include external/libpng/include
 )
endif()

if(MW_USE_STB_TRUETYPE)
  target_compile_definitions(
  Mw
  PRIVATE
  USE_STB_TRUETYPE
 )
endif()

if(MW_USE_FREETYPE2)
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

if(MW_USE_WAYLAND)
  function(scan_wayland_protocol_core)
    execute_process(
            COMMAND wayland-scanner private-code /usr/share/wayland/wayland.xml ${CMAKE_CURRENT_SOURCE_DIR}/src/backend/wayland-core-protocol.c
            OUTPUT_VARIABLE WAYLAND_SCANNER_OUTPUT
            ERROR_VARIABLE WAYLAND_SCANNER_ERROR
            ECHO_OUTPUT_VARIABLE
            ECHO_ERROR_VARIABLE
            COMMAND_ECHO STDOUT
        )
    target_sources(
        Mw
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/backend/wayland-core-protocol.c
        )
    execute_process(
            COMMAND wayland-scanner client-header /usr/share/wayland/wayland.xml ${CMAKE_CURRENT_SOURCE_DIR}/include/Mw/LowLevel/Wayland/wayland-core-protocol.h
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE WAYLAND_SCANNER_OUTPUT
            ERROR_VARIABLE WAYLAND_SCANNER_ERROR
            ECHO_OUTPUT_VARIABLE
            ECHO_ERROR_VARIABLE
            COMMAND_ECHO STDOUT
        )
  endfunction()

  function(scan_wayland_protocol tier proto ver)
    SET(proto_c ${CMAKE_CURRENT_SOURCE_DIR}/src/backend/wayland-${proto}-protocol.c)
    execute_process(
            COMMAND wayland-scanner private-code /usr/share/wayland-protocols/${tier}/${proto}/${proto}${ver}.xml ${proto_c}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE WAYLAND_SCANNER_OUTPUT
            ERROR_VARIABLE WAYLAND_SCANNER_ERROR
            ECHO_OUTPUT_VARIABLE
            ECHO_ERROR_VARIABLE
            COMMAND_ECHO STDOUT
        )
    target_sources(
        Mw
        PRIVATE
            ${proto_c}
        )
    execute_process(
            COMMAND wayland-scanner client-header /usr/share/wayland-protocols/${tier}/${proto}/${proto}${ver}.xml ${CMAKE_CURRENT_SOURCE_DIR}/include/Mw/LowLevel/Wayland/${proto}-client-protocol.h
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE WAYLAND_SCANNER_OUTPUT
            ERROR_VARIABLE WAYLAND_SCANNER_ERROR
            ECHO_OUTPUT_VARIABLE
            ECHO_ERROR_VARIABLE
            COMMAND_ECHO STDOUT
        )
  endfunction()

  check_include_files(wayland-client.h HAVE_WL_H)
  check_include_files(xkbcommon/xkbcommon.h HAVE_XKBCOMMON_H)
  check_include_files(cairo/cairo.h HAVE_CAIRO_H)
  if(HAVE_WL_H)
    if(HAVE_XKBCOMMON_H)
      if(HAVE_CAIRO_H)
        scan_wayland_protocol_core()
        scan_wayland_protocol("stable"   "xdg-shell"         "")
        scan_wayland_protocol("stable"   "viewporter"        "")
        scan_wayland_protocol("stable"   "tablet"            "-v2")
        scan_wayland_protocol("staging"  "xdg-toplevel-icon" "-v1")
        scan_wayland_protocol("staging"  "cursor-shape"      "-v1")
        scan_wayland_protocol("unstable" "xdg-decoration"    "-unstable-v1")
        scan_wayland_protocol("unstable" "primary-selection" "-unstable-v1")
        scan_wayland_protocol("unstable" "pointer-constraints" "-unstable-v1")
        scan_wayland_protocol("unstable" "relative-pointer" "-unstable-v1")

        target_sources(
                Mw
                PRIVATE
                src/backend/wayland.c
                )
        target_compile_definitions(
                    Mw
                    PRIVATE
                    USE_WAYLAND
                )
        message(STATUS "Wayland backend has been enabled")
      else()
        message(WARNING "Wayland backend has been disabled")
      endif()
    else()
      message(WARNING "Wayland backend has been disabled")
    endif()
  else()
    message(WARNING "Wayland backend has been disabled")
  endif()
endif()

if(APPLE AND MW_BUILD_OPENGL)
  target_compile_definitions(
  Mw
  PRIVATE
  GL_SILENCE_DEPRECATION
 )
endif()

target_include_directories(
 Mw
 PRIVATE
 external/libz/include
)

target_include_directories(
 Mw
 PUBLIC
 include
)

if(MW_CLASSIC_THEME)
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
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  target_sources(
  Mw
  PRIVATE
  src/backend/cocoa.m
 )
  target_compile_definitions(
  Mw
  PRIVATE
  USE_COCOA
 )
  list(APPEND LIBRARIES objc)
  target_link_options(
  Mw
  PRIVATE
  -framework Cocoa
 )
elseif(CMAKE_SYSTEM_NAME MATCHES "Retro")
  target_sources(
   Mw
   PRIVATE
   src/backend/classicmacos.c
  )
  if(PLATFORM MATCHES retroppc)
    set_target_properties(Mw PROPERTIES COMPILE_FLAGS "-ffunction-sections -mcpu=601 -Os -Wall -Wextra -Wno-unused-parameter")
    set_target_properties(Mw PROPERTIES LINK_FLAGS "-Wl,-gc-sections")
  endif()

  target_compile_definitions(
   Mw
   PRIVATE
   CLASSIC_MAC_OS
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

if(MW_BUILD_VULKAN)
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

if(MW_INSTALL_HEADERS)
  install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h"
 )
endif()

if(MW_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
