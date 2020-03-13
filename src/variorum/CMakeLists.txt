configure_file("${CMAKE_CURRENT_SOURCE_DIR}/variorum_config.h.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/variorum_config.h")

set(variorum_headers
  config_architecture.h
  variorum.h
  variorum_timers.h
  variorum_error.h
  )

set(variorum_sources
  config_architecture.c
  variorum.c
  variorum_timers.c
  variorum_error.c
  )

set(variorum_deps ""
    CACHE INTERNAL ""
  )

set(variorum_includes
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${HWLOC_INCLUDE_DIRS}
  )

if(VARIORUM_WITH_INTEL)
    add_subdirectory(Intel)
    list(APPEND variorum_headers Intel/config_intel.h ${variorum_intel_headers})
    list(APPEND variorum_sources Intel/config_intel.c)
    list(APPEND variorum_deps $<TARGET_OBJECTS:variorum_intel>)
    list(APPEND variorum_includes ${CMAKE_CURRENT_SOURCE_DIR}/Intel)
endif()

if(VARIORUM_WITH_IBM)
    add_subdirectory(IBM)
    list(APPEND variorum_headers IBM/config_ibm.h ${variorum_ibm_headers})
    list(APPEND variorum_sources IBM/config_ibm.c)
    list(APPEND variorum_deps $<TARGET_OBJECTS:variorum_ibm>)
    list(APPEND variorum_includes ${CMAKE_CURRENT_SOURCE_DIR}/IBM)
endif()

if(VARIORUM_WITH_NVIDIA)
    add_subdirectory(Nvidia)
    list(APPEND variorum_headers Nvidia/config_nvidia.h ${variorum_nvidia_headers})
    list(APPEND variorum_sources Nvidia/config_nvidia.c)
    list(APPEND variorum_deps $<TARGET_OBJECTS:variorum_nvidia>)
    list(APPEND variorum_includes ${CMAKE_CURRENT_SOURCE_DIR}/Nvidia)
endif()

if(BUILD_SHARED_LIBS)
    add_library(variorum SHARED
                ${variorum_sources}
                ${variorum_headers}
                ${variorum_deps})
else()
    add_library(variorum STATIC
                ${variorum_sources}
                ${variorum_headers}
                ${variorum_deps})
endif()

target_link_libraries(variorum PUBLIC ${HWLOC_LIBRARY})
target_link_libraries(variorum PUBLIC m)

install(TARGETS variorum
        EXPORT  variorum
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION lib)

set(variorum_install_headers
    variorum.h
  )

install(FILES ${variorum_install_headers}
        DESTINATION include)

include_directories(${variorum_includes})
