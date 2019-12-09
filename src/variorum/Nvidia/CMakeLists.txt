set(variorum_nvidia_headers
  ${CMAKE_CURRENT_SOURCE_DIR}/Volta.h
  ${CMAKE_CURRENT_SOURCE_DIR}/power_features.h
  CACHE INTERNAL "")

set(variorum_nvidia_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/Volta.c
  ${CMAKE_CURRENT_SOURCE_DIR}/power_features.c
  CACHE INTERNAL "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${variorum_includes})

add_library(variorum_nvidia OBJECT
            ${variorum_nvidia_sources}
            ${variorum_nvidia_headers})

### Shared libraries need PIC
set_property(TARGET ${variorum_nvidia} PROPERTY POSITION_INDEPENDENT_CODE 1)
