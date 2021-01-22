.. # Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
.. # Variorum Project Developers. See the top-level LICENSE file for details.
.. #
.. # SPDX-License-Identifier: MIT


Example Programs
================

Variorum provides some examples in the `examples/` directory.

.. note::
    All example codes have a print and print_verbose version showcasing the
    different printouts supported by Variorum.

Print Power Limits
^^^^^^^^^^^^^^^^^^

The example below gets the power limits of the platform. The ``print`` API
prints the output in tabular format that can be filtered and parsed by a data
analysis framework, such as `R` or `Python`.

.. literalinclude:: ../../examples/variorum-print-power-limits-example.c
    :language: c

On an Intel platform, the output of this example should be similar to the
following:

.. code::

    _LOG_VARIORUM_ENTER:~/variorum/src/variorum/variorum.c:variorum_print_power_limits::143
    _PACKAGE_POWER_LIMITS Offset Host Socket PowerLimBits Watts1 Seconds1 Watts2 Seconds2
    _PACKAGE_POWER_LIMITS 0x610 thompson 0 0x7851000158438 135.000000 1.000000 162.000000 0.007812
    _PACKAGE_POWER_LIMITS 0x610 thompson 1 0x7851000158438 135.000000 1.000000 162.000000 0.007812
    _DRAM_POWER_LIMIT Offset Host Socket PowerLimBits Watts Seconds
    _DRAM_POWER_LIMIT 0x618 thompson 0 0x0 0.000000 0.000977
    _DRAM_POWER_LIMIT 0x618 thompson 1 0x0 0.000000 0.000977
    _PACKAGE_POWER_INFO Offset Host Socket Bits MaxPower MinPower MaxTimeWindow ThermPower
    _PACKAGE_POWER_INFO 0x614 thompson 0 0x2f087001380438 270.000000 39.000000 40.000000 135.000000
    _PACKAGE_POWER_INFO 0x614 thompson 1 0x2f087001380438 270.000000 39.000000 40.000000 135.000000
    _RAPL_POWER_UNITS Offset Host Socket Bits WattsUnit JoulesUnit SecondsUnit
    _RAPL_POWER_UNITS 0x606 thompson 0 0xa0e03 0.125000 0.000061 0.000977
    _RAPL_POWER_UNITS 0x606 thompson 1 0xa0e03 0.125000 0.000061 0.000977
    _LOG_VARIORUM_EXIT:~/variorum/src/variorum/variorum.c:variorum_print_power_limits::165

On an IBM Power9 platform, the output may look similar to:

.. code::

    _LOG_VARIORUM_ENTER:~/variorum/src/variorum/variorum.c:variorum_print_power_limits::143
    _POWERCAP Host CurrentWatts MaxWatts MinWatts PSR_CPU_to_GPU_0 PSR_CPU_to_GPU_8
    _POWERCAP lassen3 3050 3050 500 100 100
    _LOG_VARIORUM_EXIT:~/variorum/src/variorum/variorum.c:variorum_print_power_limits::165

    On an Nvidia GPU platform, the output may look similar to:

.. code::

    _LOG_VARIORUM_ENTER:~/variorum/src/variorum/variorum.c:variorum_print_power_limits::143
    _GPU_POWER_LIMIT Host Socket Device PowerLimit
    _GPU_POWER_LIMIT lassen1 0 0 300.000
    _GPU_POWER_LIMIT lassen1 0 1 300.000
    _GPU_POWER_LIMIT lassen1 1 2 300.000
    _GPU_POWER_LIMIT lassen1 1 3 300.000
    _LOG_VARIORUM_EXIT:~/variorum/src/variorum/variorum.c:variorum_print_power_limits::165

Print Verbose Power Limits
^^^^^^^^^^^^^^^^^^^^^^^^^^

The example below gets the power limits of the platform. The ``print_verbose``
API prints the output in verbose format that is more human-readable (with units,
titles, etc.).

.. literalinclude:: ../../examples/variorum-print-verbose-power-limits-example.c
    :language: c

On an Intel platform, the output of this example should be similar to the
following:

.. code::

    _LOG_VARIORUM_ENTER:~/variorum/src/variorum/variorum.c:variorum_print_verbose_power_limits::180
    _PACKAGE_POWER_LIMIT Offset: 0x610, Host: thompson, Socket: 0, Bits: 0x7851000158438, PowerLimit1: 135.000000 W, TimeWindow1: 1.000000 sec, PowerLimit2: 162.000000 W, TimeWindow2: 0.007812 sec
    _PACKAGE_POWER_LIMIT Offset: 0x610, Host: thompson, Socket: 1, Bits: 0x7851000158438, PowerLimit1: 135.000000 W, TimeWindow1: 1.000000 sec, PowerLimit2: 162.000000 W, TimeWindow2: 0.007812 sec
    _DRAM_POWER_LIMIT Offset: 0x618, Host: thompson, Socket: 0, Bits: 0x0, PowerLimit: 0.000000 W, TimeWindow: 0.000977 sec
    _DRAM_POWER_LIMIT Offset: 0x618, Host: thompson, Socket: 1, Bits: 0x0, PowerLimit: 0.000000 W, TimeWindow: 0.000977 sec
    _PACKAGE_POWER_INFO Offset: 0x614, Host: thompson, Socket: 0, Bits: 0x2f087001380438, MaxPower: 270.000000 W, MinPower: 39.000000 W, MaxWindow: 40.000000 sec, ThermPower: 135.000000 W
    _PACKAGE_POWER_INFO Offset: 0x614, Host: thompson, Socket: 1, Bits: 0x2f087001380438, MaxPower: 270.000000 W, MinPower: 39.000000 W, MaxWindow: 40.000000 sec, ThermPower: 135.000000 W
    _RAPL_POWER_UNITS Offset: 0x606, Host: thompson, Socket: 0, Bits: 0xa0e03, PowerUnit: 0.125000 W, EnergyUnit: 0.000061 J, TimeUnit: 0.000977 sec
    _RAPL_POWER_UNITS Offset: 0x606, Host: thompson, Socket: 1, Bits: 0xa0e03, PowerUnit: 0.125000 W, EnergyUnit: 0.000061 J, TimeUnit: 0.000977 sec
    _LOG_VARIORUM_EXIT:~/variorum/src/variorum/variorum.c:variorum_print_verbose_power_limits::202

On an IBM platform, the output may look similar to:

.. code::

    _LOG_VARIORUM_ENTER:~/variorum/src/variorum/variorum.c:variorum_print_verbose_power_limits::180
    _POWERCAP Host: lassen3, CurrentWatts: 3050 W, MaxWatts: 3050 W, MinWatts: 500 W, PSR_CPU_to_GPU_0: 100%, PSR_CPU_to_GPU_8: 100%
    _LOG_VARIORUM_EXIT:~/variorum/src/variorum/variorum.c:variorum_print_verbose_power_limits::202

On an Nvidia platform, the output may look similar to:

.. code::

    _LOG_VARIORUM_ENTER:~/variorum/src/variorum/variorum.c:variorum_print_verbose_power_limits::180
    _GPU_POWER_LIMIT Host: lassen1, Socket: 0, Device_ID: 0, Power_Limit: 300.000
    _GPU_POWER_LIMIT Host: lassen1, Socket: 0, Device_ID: 1, Power_Limit: 300.000
    _GPU_POWER_LIMIT Host: lassen1, Socket: 1, Device_ID: 2, Power_Limit: 300.000
    _GPU_POWER_LIMIT Host: lassen1, Socket: 1, Device_ID: 3, Power_Limit: 300.000
    _LOG_VARIORUM_EXIT:~/variorum/src/variorum/variorum.c:variorum_print_verbose_power_limits::202
