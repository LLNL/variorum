LIBVARIORUM
===========

Welcome to Libvariorum, a friendly (well, friendlier) interface to many of the
model-specific registers in Intel processors. Now with PCI configuration
register support for some Intel hardware.

version 0.0.0


Last Update
-----------
17 July 2018


Webpages
--------
http://software.llnl.gov/libvariorum <br>
https://github.com/llnl/libvariorum


Overview
--------

Libvariorum provides an interface to accessing the model-specific registers (MSRs)
on Intel platforms, which provide privileged functionality for monitoring and
controlling various CPU features.


Installation
------------

Installation is simple. You will need [CMAKE](http://www.cmake.org) version 2.8
or higher and GCC. Libvariorum does not support in-source builds. In most cases,
the installation is as follows:

    $ cd libvariorum/
    $ mkdir build && cd build/
    $ cmake -DCMAKE_INSTALL_PREFIX=../install ../src
    $ make
    $ make install

Currently supported architectures are Intel Xeons (server processors). The
library technically supports all processors based on these architectures, but
some features may be missing from client products.

Supported Architectures:

	2A (Sandy Bridge)
	3E (Ivy Bridge)
	3E (Haswell)
	4F (Broadwell)
	9E (Kaby Lake)
	55 (Skylake)

If you are unsure of your architecture number, check the "model" field in `lscpu`
or `/proc/cpuinfo` (note that it will not be in hexadecimal).


Testing
-------
From within the build directory, tests can be executed with the
`make test` target.


Support Additional Intel Platforms
----------------------------------
1) Under `Intel/` directory, create a `.h` and `.c` header and source file for the
respective platform. This will contain features specific to that platform,
which may or may not exist in previous generations. 
2) Modify `Intel/config_intel.c` to set the function pointers for the
respective platform.
3) Include the new header file in `Intel/config_architecture.h`.


Notes
-----

This software depends on the files `/dev/cpu/*/msr` being present with R/W
permissions. Recent kernels require additional capabilities. We have found it
easier to use our own [MSR-SAFE](https://github.com/LLNL/msr-safe) kernel
module, but running as root (or going through the bother of adding the
capabilities to the binaries) is another option.

If you need PCI configuration register (CSR) support in Libvariorum, you MUST have
CSR-SAFE installed. This code is not currently on Github -- you will need to
request it.

For sample code, see the `examples/` directory.


Bug Tracker
-----------

Bugs and feature requests are being tracked on the [Institutional Jira](https://lc.llnl.gov/jira/projects/VARIORUM/summary).


Contact
-------

Barry Rountree, Project Lead, <rountree@llnl.gov> <br>
Stephanie Labasan, Developer, <labasan1@llnl.gov>

Please feel free to contact the developers with any questions or feedback.

We are collecting names of those who have previously contributed to libmsr over
the years. See the current list in the `AUTHORS` file. Please contact the
developers to have your name added to the list.


Release
-------

This has not been released yet.
