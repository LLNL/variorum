# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

add_custom_target(docs)

if(DOXYGEN_FOUND)
    add_custom_target(doxygen_docs)
    add_dependencies(docs doxygen_docs)
endif()


if(SPHINX_FOUND)
    add_custom_target(sphinx_docs)
    add_dependencies(docs sphinx_docs)
endif()

macro(add_doxygen_target doxygen_target_name)
    message(STATUS "Creating doxygen docs target ${doxygen_target_name}")

    # add a target to generate API documentation with Doxygen
    add_custom_target(${doxygen_target_name}
                     ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                     COMMENT "Generating API documentation with Doxygen" VERBATIM)

    add_dependencies(doxygen_docs ${doxygen_target_name})

    install(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} ${doxygen_target_name} WORKING_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}\")")

    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html"
            DESTINATION docs/doxygen/${doxygen_target_name})
endmacro(add_doxygen_target)

macro(add_sphinx_target sphinx_target_name)
    message(STATUS "Creating sphinx docs target ${sphinx_target_name}")
    # configured documentation tools and intermediate build results
    set(SPHINX_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")

    # Sphinx cache with pickled ReST documents
    set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_doctrees")

    # HTML output directory
    set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/html")

    # support both a cmake config-d sphinx input file (config.py.in)
    # and direct use of a config.py file.
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in")

        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
                       "${SPHINX_BUILD_DIR}/conf.py"
                       @ONLY)

        add_custom_target(${sphinx_target_name}
                          ${SPHINX_EXECUTABLE}
                          -q -b html
                          #-W disable warn on error for now, while our sphinx env is still in flux
                          -c "${SPHINX_BUILD_DIR}"
                          -d "${SPHINX_CACHE_DIR}"
                          "${CMAKE_CURRENT_SOURCE_DIR}"
                          "${SPHINX_HTML_DIR}"
                          COMMENT "Building HTML documentation with Sphinx"
                          DEPENDS ${deps})
    else()
        add_custom_target(${sphinx_target_name}
                          ${SPHINX_EXECUTABLE}
                          -q -b html
                          #-W disable warn on error for now, while our sphinx env is still in flux
                          -d "${SPHINX_CACHE_DIR}"
                          "${CMAKE_CURRENT_SOURCE_DIR}"
                          "${SPHINX_HTML_DIR}"
                          COMMENT "Building HTML documentation with Sphinx"
                          DEPENDS ${deps})
    endif()
    # hook our new target into the docs dependency chain
    add_dependencies(sphinx_docs ${sphinx_target_name})

    ######
    # This snippet makes sure if we do a make install w/o the optional "docs"
    # target built, it will be built during the install process.
    ######

    install(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} ${sphinx_target_name} WORKING_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}\")")

    install(DIRECTORY "${SPHINX_HTML_DIR}"
            DESTINATION "docs/sphinx/${sphinx_target_name}")
endmacro(add_sphinx_target)
