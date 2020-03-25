set(VARIORUM_INCLUDE_DIRS "${VARIORUM_INSTALL_PREFIX}/include")

# create convenience target that bundles all reg variorum deps
add_library(variorum::variorum INTERFACE IMPORTED)

set_property(TARGET variorum::variorum
             APPEND PROPERTY
             INTERFACE_INCLUDE_DIRECTORIES "${VARIORUM_INSTALL_PREFIX}/include/")

set_property(TARGET variorum::variorum
             APPEND PROPERTY
             INTERFACE_INCLUDE_DIRECTORIES "${VARIORUM_INSTALL_PREFIX}/include")

set_property(TARGET variorum::variorum
             PROPERTY INTERFACE_LINK_LIBRARIES
             variorum)

#set_property(TARGET variorum::variorum
#             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
#             hwloc)

if(NOT Variorum_FIND_QUIETLY)
    message(STATUS "VARIORUM_INSTALL_PREFIX = ${VARIORUM_INSTALL_PREFIX}")
    message(STATUS "VARIORUM_INCLUDE_DIRS   = ${VARIORUM_INCLUDE_DIRS}")

    set(_print_targets "")
    message(STATUS "Variorum imported targets: ${_print_targets}")
    unset(_print_targets)
endif()

