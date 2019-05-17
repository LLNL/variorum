Variorum Overview
=================

Variorum is an evolution of an existing open-source library developed at LLNL
known as ``libmsr``. Variorum is a production-grade software infrastructure for
exposing low-level control (and monitoring) of a system's underlying hardware
features through a secure whitelist mechanism. It can easily be extended to
different hardware devices, as well as different generations within a
particular device. More specifically, Variorum's flexible design supports a set
of features that may exist on one generation of hardware, but not on another.

Requirements
------------
To guide the development of Variorum, we focused on a set of important
requirements extracted from our learnings with the development of ``libmsr``.
Here are Variorum's requirements:

Create device-agnostic APIs.
  * We do not want to require the user to have to know or understand how to interface with each device. The library is built for a target architecture, which may be heterogeneous, and can collect data from each device through a single front-facing interface.

Provide a simple interface.
  * We want users and tool developers to not only collect information from the underlying hardware, but also to have the ability to control various features from user space with a whitelist. Traditionally, control over some processor features, such as applying a power cap, is limited to those with privileged access. At LLNL, we developed an open-source kernel module known as ``msr-safe`` as a means of providing access to Intel-specific features and controls from user space by whitelisting MSRs. With Variorum, we plan to provide similar access to other hardware devices through a safe and secure model similar to msr-safe.

Ease in extending to new devices and generations within a device.
  * Variorum makes it easy to support new features, deprecate old features among generations of devices, and adapt features that may have different domains of control from one generation to another (i.e., sockets, cores, threads).


System Diagram
--------------
..  image:: images/VariorumSystemDesign.png
    :height: 600px
    :align: center


Dependencies
------------

hwloc (Required)
""""""""""""""""
`hwloc <https://www.open-mpi.org/projects/hwloc/>`_ is an open-source project
providing a portable abstraction of the hierarchical topology of modern
architectures.
