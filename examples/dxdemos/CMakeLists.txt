file(
 GLOB
 EXAMPLES_DIRECTX_SOURCES
 *.c
)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../resource/FindDirectX.cmake)

if(${DIRECTX_FOUND})
  foreach(PATH IN LISTS EXAMPLES_DIRECTX_SOURCES)
    get_filename_component(TARGET ${PATH} NAME_WE)
    add_executable(${TARGET} MACOSX_BUNDLE ${PATH})
    target_include_directories(
  ${TARGET}
  PRIVATE
  ${DIRECTX_INCLUDE_DIRS}
 )
    target_link_libraries(
   ${TARGET}
   PRIVATE
   Mw
   ${DIRECTX_D3DX9_LIBRARIES}
  )
  endforeach()
endif()
