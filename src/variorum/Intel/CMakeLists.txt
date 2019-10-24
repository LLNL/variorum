set(variorum_intel_headers
  ${CMAKE_CURRENT_SOURCE_DIR}/clocks_features.h
  ${CMAKE_CURRENT_SOURCE_DIR}/counters_features.h
  ${CMAKE_CURRENT_SOURCE_DIR}/power_features.h
  ${CMAKE_CURRENT_SOURCE_DIR}/thermal_features.h
  ${CMAKE_CURRENT_SOURCE_DIR}/misc_features.h
  ${CMAKE_CURRENT_SOURCE_DIR}/SandyBridge_2A.h
  ${CMAKE_CURRENT_SOURCE_DIR}/IvyBridge_3E.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Haswell_3F.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Broadwell_4F.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Skylake_55.h
  ${CMAKE_CURRENT_SOURCE_DIR}/KabyLake_9E.h
  ${CMAKE_CURRENT_SOURCE_DIR}/msr_core.h
  ${CMAKE_CURRENT_SOURCE_DIR}/variorum_cpuid.h
  CACHE INTERNAL "")

set(variorum_intel_sources
  ${CMAKE_CURRENT_SOURCE_DIR}/clocks_features.c
  ${CMAKE_CURRENT_SOURCE_DIR}/counters_features.c
  ${CMAKE_CURRENT_SOURCE_DIR}/power_features.c
  ${CMAKE_CURRENT_SOURCE_DIR}/thermal_features.c
  ${CMAKE_CURRENT_SOURCE_DIR}/misc_features.c
  ${CMAKE_CURRENT_SOURCE_DIR}/SandyBridge_2A.c
  ${CMAKE_CURRENT_SOURCE_DIR}/IvyBridge_3E.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Haswell_3F.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Broadwell_4F.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Skylake_55.c
  ${CMAKE_CURRENT_SOURCE_DIR}/KabyLake_9E.c
  ${CMAKE_CURRENT_SOURCE_DIR}/msr_core.c
  ${CMAKE_CURRENT_SOURCE_DIR}/variorum_cpuid.c
  CACHE INTERNAL "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${variorum_includes})

add_library(variorum_intel OBJECT
            ${variorum_intel_sources}
            ${variorum_intel_headers})

### Shared libraries need PIC
set_property(TARGET ${variorum_intel} PROPERTY POSITION_INDEPENDENT_CODE 1)
