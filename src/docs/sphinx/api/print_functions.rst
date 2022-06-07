.. # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##########################
 Variorum Print Functions
##########################

Variorum provides the following high-level functions for printing
the value of various features. For each feature, there is a ``print`` and
``print_verbose`` API, which will print the metrics in different output
formats. The ``print`` API prints the output in tabular format that can be
filtered and parsed by a data analysis framework, such as ``R`` or ``Python``.
The ``print_verbose`` API prints the output in verbose format that is more
human-readable (with units, titles, etc.). See :doc:`Examples` for sample
output formats supported by Variorum.

Defined in ``variorum/variorum.h``.

.. doxygenfunction:: variorum_print_verbose_power_limit

.. doxygenfunction:: variorum_print_power_limit

.. doxygenfunction:: variorum_print_verbose_thermals

.. doxygenfunction:: variorum_print_thermals

.. doxygenfunction:: variorum_print_verbose_counters

.. doxygenfunction:: variorum_print_counters

.. doxygenfunction:: variorum_print_verbose_power

.. doxygenfunction:: variorum_print_power

.. doxygenfunction:: variorum_print_verbose_frequency

.. doxygenfunction:: variorum_print_frequency

.. doxygenfunction:: variorum_print_hyperthreading

.. doxygenfunction:: variorum_print_topology

.. doxygenfunction:: variorum_print_features

.. doxygenfunction:: variorum_print_turbo

.. doxygenfunction:: variorum_print_verbose_gpu_utilization

.. doxygenfunction:: variorum_print_gpu_utilization

.. doxygenfunction:: variorum_print_available_frequencies

.. doxygenfunction:: variorum_print_energy

.. doxygenfunction:: variorum_poll_power

.. doxygenfunction:: variorum_monitoring

