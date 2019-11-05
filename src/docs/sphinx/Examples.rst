.. # Copyright 2019 Lawrence Livermore National Security, LLC and other
.. # Variorum Project Developers. See the top-level LICENSE file for details.
.. #
.. # SPDX-License-Identifier: MIT


Example Programs
================

Variorum provides some examples in the `examples/` directory.

.. note::
    All example codes have a print and dump version showcasing the different
    printouts supported by Variorum.

Dump Thermals
^^^^^^^^^^^^^

The example below gets the thermal temperatures before and after running some
application. The ``dump`` API prints the output in CSV format that is easily
parsable by some data analysis framework, like `R` or `Python`.

.. literalinclude:: ../../examples/ex-dump-thermals.c
    :language: c

Print Thermals
^^^^^^^^^^^^^^

The example below gets the thermal temperatures before and after running some
application. The ``print`` API prints the output in verbose format that is more
human-readable (with units, titles).

.. literalinclude:: ../../examples/ex-print-thermals.c
    :language: c
