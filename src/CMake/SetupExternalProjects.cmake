include(ExternalProject)
ExternalProject_Add(libhwloc
  GIT_REPOSITORY https://github.com/open-mpi/hwloc.git
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/thirdparty_builtin/hwloc
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/thirdparty_builtin/hwloc/autogen.sh && ${CMAKE_CURRENT_BINARY_DIR}/thirdparty_builtin/hwloc/configure --prefix=${PROJECT_BINARY_DIR}/thirdparty_builtin/libhwloc-prefix
  PREFIX ${PROJECT_BINARY_DIR}/thirdparty_builtin/libhwloc-prefix
  BUILD_COMMAND make
  INSTALL_COMMAND
  BUILD_IN_SOURCE 1
)

set(HWLOC_DIR
  ${PROJECT_BINARY_DIR}
  CACHE INTERNAL "")

set(HWLOC_INCLUDE_DIRS
  ${PROJECT_BINARY_DIR}/thirdparty_builtin/libhwloc-prefix/include
  CACHE INTERNAL "")

set(HWLOC_LIBRARY
  ${PROJECT_BINARY_DIR}/thirdparty_builtin/libhwloc-prefix/lib/${CMAKE_SHARED_LIBRARY_PREFIX}hwloc${CMAKE_SHARED_LIBRARY_SUFFIX}
  CACHE INTERNAL "")

include_directories(${HWLOC_INCLUDE_DIRS})
