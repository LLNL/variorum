# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(PYTHON_EXAMPLES
    variorum-cap-best-effort-node-power-limit-python-example
)

set(PY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/examples/pyVariorum)

message(STATUS "Adding variorum Python examples")
foreach(EXAMPLE ${PYTHON_EXAMPLES})
    message(STATUS " [*] Adding Python example: ${EXAMPLE}")
    install(FILES ${EXAMPLE}.py DESTINATION ${PY_INSTALL_PATH})
endforeach()
