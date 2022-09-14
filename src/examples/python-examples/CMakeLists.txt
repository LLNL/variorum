# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(PYTHON_EXAMPLES
    variorum-cap-best-effort-node-power-limit-python-example.py
    variorum-cap-gpu-power-ratio-python-example.py
    variorum-cap-socket-power-limit-python-example.py
    variorum-get-current-version-python-example.py
    variorum-get-node-power-domain-info-json-python-example.py
    variorum-get-node-power-json-python-example.py
    variorum-print-available-frequencies-python-example.py
    variorum-print-counters-python-example.py
    variorum-print-features-python-example.py
    variorum-print-frequency-python-example.py
    variorum-print-gpu-utilization-python-example.py
    variorum-print-hyperthreading-python-example.py
    variorum-print-power-limit-python-example.py
    variorum-print-power-python-example.py
    variorum-print-thermals-python-example.py
    variorum-print-topology-python-example.py
    variorum-print-turbo-python-example.py
    variorum-print-verbose-counters-python-example.py
    variorum-print-verbose-frequency-python-example.py
    variorum-print-verbose-gpu-utilization-python-example.py
    variorum-print-verbose-power-limit-python-example.py
    variorum-print-verbose-power-python-example.py
    variorum-print-verbose-thermals-python-example.py
)

message(STATUS "Adding variorum Python examples")
foreach(EXAMPLE ${PYTHON_EXAMPLES})
    message(STATUS " [*] Adding Python example: ${EXAMPLE}")
    configure_file("${EXAMPLE}" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)
endforeach()
