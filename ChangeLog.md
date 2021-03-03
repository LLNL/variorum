# Wed Mar 3 2021 Stephanie Brink <brink2@llnl.gov> v0.4.0
* Add support for ARM Juno architecture
* Add top-level API to query available frequencies, add support for Intel and ARM
* Add JANSSON-C dependency
* Introduce JSON API to return monitoring data into a structure
* Update variorum to build and run with both msr-safe < v1.5 (msr_whitelist)
  msr-safe >= v1.5 (msr_allowlist)
* Add best effort node power cap support for Intel Broadwell and IBM
* Build option to enable all warnings, cleanup compiler warnings
* Default build type switched from Release to RelWithDebInfo
* Consistent formatting for verbose printouts across architectures
* Update year in license headers
* Update docs throughout: JSON API, best effort power cap, architectures
  supported, add sample output to examples
* Rename set functions to cap throughout top-level API (and internally in
  architecture-level functions) to clarify that the setting is an upper bound;
  instantaneous reads on these metrics may be at or below the value

# Thu Apr 23 2020 Stephanie Brink <brink2@llnl.gov> v0.3.0
* Setup unit test infrastructure, test on gitlab to leverage msr-safe and OPAL
  access
* Update return code for top-level variorum APIs from -1 to 0, enables us to
  run unit tests on any architecture
* Fix poll_power and monitoring segfaults
* Start adding python utilities to verify system environments (i.e. msr-safe,
  OPAL)
* Rename variorum_set_topology to variorum_get_topology
* Rename variorum examples
* Add example integrations with cmake/make-build systems and c/cpp/fortran
  codes, use these in CI workflow to test install
* Use spack mirrors in CI workflow to avoid failed fetch commands
* New mailing lists: variorum-users and variorum-maintainers
* Nvidia port: use node topology to map GPU to sockets
* Fix for building variorum as static library
* Add source code (c/cpp/h/python) format check to CI workflow
* Add Contributor Covenant Code of Conduct

# Fri Mar 13 2020 Stephanie Brink <brink2@llnl.gov> v0.2.0
* Add support for Nvidia platform -- Volta architecture
* Add ECP 2020 tutorial presentation
* Prefix front-facing APIs with variorum_
* Bug fix for computing power usage values
* On Intel platforms, translate both power limit time windows (not just TW1)
* Fix hwloc topology mapping (use logical indices, not OS)
* Integrate Travis and GitLab for CI
* Modify IBM powmon formatting into columns
* Update license year for 2020

# Wed Nov 11 2019 Stephanie Brink <brink2@llnl.gov> v0.1.0
* Initial release to <https://github.com/llnl/variorum>!
* Includes support for Intel platform -- Sandy Bridge, Ivy Bridge, Haswell,
  Broadwell, Skylake, and Kaby Lake architectures
* Includes support for IBM platform -- Power9 architecture
* Add powmon command line sample for monitoring performance counters during
  application execution
* Initial RTD at <https://variorum.readthedocs.io>
* Rename libvariorum to variorum
* Bug fix for computing power from energy on Intel
* Add developer docs for supporting new platforms
* Add info on IBM sensors, OCC, and power shifting ratio
* Rename front-facing APIs from print_*() to print_verbose_*() and dump_*() to
  print_*()

# Wed Jul 19 2017 Stephanie Labasan <labasan1@llnl.gov> v0.0.0
* Initial release to libvariorum repo on internal Bitbucket.
* Base Sphinx documentation taken from SDAV Alpine project. Still has some
  legacy info as an example..
* Use Breathe to interpret Doxygen source code comments for documentation on
  Sphinx webpage.
* Download and build hwloc dependency.
* Include googletest in libvariorum source code and build dependency.
* Include a couple googletests checking the presence, permissions, and validity
  of the msr and msr-safe drivers and the msr whitelist.
* Include a host configuration file for LLNL's catalyst system using Intel's
  compilers to generate a CMakeCache.txt.
* Add support for the following features: print/dump_power(),
  print/dump_counters(), print/dump_thermals(), print/dump_package_frequency(),
  print/dump_power_limits(), print_topology(), print_features().
* Write examples for the above set of features.
* Add offset lists for Ivy Bridge, Broadwell, and Kaby Lake.
