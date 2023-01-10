..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

#################
 Var CLI Utility
#################

The ``var`` command has many *subcommands*.

``var help``

.. code-block:: console
   usage: var [-h] [-V] [-v]
           {avail,debug,help,monitor,query,read,setcap,write} ...

    CLI for variorum library.

    optional arguments:
      -h, --help            show this help message and exit
      -V, --version         show version number and exit
      -v, --verbose         print additional output during builds

    subcommands:
      {avail,debug,help,monitor,query,read,setcap,write}
        avail               List available features
        debug               Variorum debug utility
        help                Get help on var and its commands
        monitor             Sample performance counters
        query               Query domain features
        read                Read value from register
        setcap              Set a frequency or power cap
        write               Write a value to a register


****************************
 Listing available features
****************************

``var avail``

***************************
 Write value to a register
***************************

``var write -p 10 0x610 0x50``

************************
 Monitor an application
************************

``var monitor -a "myexec -t 10"``

******************************
 Set a frequency or power cap
******************************

Need to verify frequency or power for the domain

``var setcap frequency 10``

************
 Debug mode
************

Debugging msr-safe permissions, OPAL permissions, etc.

``var debug``

************
 Testing
************

.. program-output:: python -V

.. program-output:: echo $PATH
   :shell:

.. program-output:: var help
