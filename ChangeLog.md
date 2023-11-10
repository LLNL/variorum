# Tue Jun 13 2022 Stephanie Brink <brink2@llnl.gov> v0.7.0
Version 0.7.0 is a major release.

### New features
* Enable heterogeneous build support for two architectures
* Support for new architectures: ARM Neoverse N1 platform, Sapphire Rapids
* Add power capping API for NVIDIA, AMD and Intel GPUs
* Add get power limit for intel GPUs

### Documentation updates
* add links to youtube video and recordings from ECP Tutorial 2023
* Change knob to dial in documentation
* Update Variorum Overview Figure
* Upload new logo

### Bug fixes
* Fix fortran error with gfortran > 10
* Fix issues coming up with DENABLE_WARNINGS=On on AMD CPU port
* Fix AMD build errors: add missing headers, undefined variables, etc.
* Fix inconsistent spacing issue in powmon
* Fix for multiplatform build (ARM+NVIDIA) error/warnings
* Remove the undefined variable in AMD debug build

### Internal changes
* Powmon rehaul
* Fix for JSON example memory leak
* Remove unnecessary call to obtain number of sockets from JSON example
* Update filenames and low-level function names, prefix power_features.* with architecture name
* Replace existing RSMI API call with the correct API to get GPU busy %
* Resolve missing non-optional arguments in control examples

# Thu Sept 15 2022 Stephanie Brink <brink2@llnl.gov> v0.6.0
Version 0.6.0 is a major release.

### New features
* Add python wrappers and python module pyVariorum
* Support for new architectures: Intel discrete GPUs, AMD GPUs through ROCm-SMI

### API changes
* Update JSON APIs to use char* instead of json_t*
* Creates new variorum_topology.h header to expose get topology APIs

### Other notable changes
* VARIORUM_LOG moved from a build variable to an environment variable
* Update architecture-related names to be more granular, for example,
  VARIORUM_WITH_AMD became VARIORUM_WITH_AMD_CPU and VARIORUM_WITH_AMD_GPU, and
  SandyBridge_2A.h became Intel_06_2A.h
* Standardized examples with -h and -v flags, for printing the help menu and
  Variorum version, respectively

# Wed Jun 22 2022 Stephanie Brink <brink2@llnl.gov> v0.5.0
Version 0.5.0 is a major release.

### New features
* Support for new architectures: AMD CPUs, Intel Ice Lake
* Add examples for integration Variorum into OpenMP and MPI programs, adds
  dependency on rankstr library for MPI examples (optional)

### API changes
* Rename clock_speed to frequency and power_limits to power_limit throughout
  Variorum API
* Report info about power domains through JSON, such as what can be measured,
  what can be measured and capped (along with limits), and what cannot be
  measured or capped (unsupported).
* Deprecate set_and_verify API and removes associated example
* Extend JSON API and Best Effort Node Power Cap to ARM, AMD, and Intel
  platforms, IBM already supported

### Other notable changes
* Use getopt to parse arguments in examples
* CI migrated from Travis to GitLab
* Add presentations from ECP 2021 tutorial seriess
* Extend GitLab build matrix to build across different GCC versions
* Refactor msr_core.[ch] into its own directory as it is used by both Intel and
  AMD ports
* Improved docs: add AMD and ARM docs, rankstr dependency, re-organize
  hierarchy, update system digram, add pages for categories of APIs, add info
  on Argo, PowerStack, EEHPC WG, PowerAPI, and Variorum integrations
* License header check as new unit test

# Fri Apr 2 2021 Stephanie Brink <brink2@llnl.gov> v0.4.1
* Add docs for NVIDIA port
* Add units to tabular and JSON output formats
* Rename dump and print internal functions to print and print_verbose,
  respectively
* Standardize sphinx docs with rstfmt python package
* Update variorum system design figure in docs
* Speed up travis builds with parallel make

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
