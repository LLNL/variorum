# First check for user-specified JANSSON_DIR
if(JANSSON_DIR)
    MESSAGE(STATUS "Looking for JANSSON using JANSSON_DIR = ${JANSSON_DIR}")

    set(JANSSON_FOUND TRUE)
    set(JANSSON_INCLUDE_DIRS ${JANSSON_DIR}/include)
    set(JANSSON_LIBRARY ${JANSSON_DIR}/lib/libjansson.so)

    set(JANSSON_DIR ${JANSSON_DIR} CACHE PATH "" FORCE)

    message(STATUS "FOUND jansson at ${JANSSON_DIR}")
    message(STATUS " [*] JANSSON_INCLUDE_DIRS = ${JANSSON_INCLUDE_DIRS}")
    message(STATUS " [*] WLOC_LIBRARY = ${JANSSON_LIBRARY}")
# If JANSSON_DIR not specified, then try to automatically find the JANSSON header
# and library
elseif(NOT JANSSON_DIR)
    find_path(JANSSON_INCLUDE_DIRS
        NAMES jansson.h
    )

    find_library(JANSSON_LIBRARY
        NAMES libjansson.so
    )

    if(JANSSON_INCLUDE_DIRS AND JANSSON_LIBRARY)
        set(JANSSON_FOUND TRUE)
        message(STATUS "FOUND jansson using find_library()")
        message(STATUS " [*] JANSSON_INCLUDE_DIRS = ${JANSSON_INCLUDE_DIRS}")
        message(STATUS " [*] JANSSON_LIBRARY = ${JANSSON_LIBRARY}")
    endif()
endif()


# Abort if all methods fail
if(NOT JANSSON_FOUND)
    MESSAGE(FATAL_ERROR "Jansson support needed for json")
endif()
