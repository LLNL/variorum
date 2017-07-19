include(ExternalProject)
message(STATUS " [+] Adding external project: hwloc")
ExternalProject_Add(libhwloc
  GIT_REPOSITORY https://github.com/open-mpi/hwloc.git
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/thirdparty_builtin/hwloc
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/thirdparty_builtin/hwloc/autogen.sh && ${CMAKE_CURRENT_BINARY_DIR}/thirdparty_builtin/hwloc/configure --prefix=${PROJECT_BINARY_DIR}/thirdparty_builtin/libhwloc-prefix
  PREFIX ${PROJECT_BINARY_DIR}/thirdparty_builtin/libhwloc-prefix
  BUILD_COMMAND make
  INSTALL_COMMAND make install
  BUILD_IN_SOURCE 1
)

set(hwloc_includes
  ${PROJECT_BINARY_DIR}/thirdparty_builtin/libhwloc-prefix/include
  CACHE INTERNAL "")

set(hwloc_library
  ${PROJECT_BINARY_DIR}/thirdparty_builtin/libhwloc-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}hwloc${CMAKE_SHARED_LIBRARY_SUFFIX}
  CACHE INTERNAL "")

include_directories(${hwloc_includes})

ExternalProject_Add_Step(libhwloc CopyToLib
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_BINARY_DIR}/thirdparty_builtin/libhwloc-prefix/lib/ ${CMAKE_INSTALL_PREFIX}/lib/hwloc
  DEPENDEES install
)
