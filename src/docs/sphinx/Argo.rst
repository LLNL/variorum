..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

##################
 ECP Argo Project
##################

Argo is a collaborative project between Argonne National Laboratory and Lawrence
Livermore National Laboratory. It is funded by the U.S. Department of Energy as
part of the Exascale Computing Project (ECP).The goal of the Argo project is to
augment and optimize existing Operating Systems/Runtime components for use in
production HPC systems, providing portable, open source, integrated software
that improves the performance and scalability of and that offers increased
functionality to exascale applications and runtime systems. Argo software is
developed as a toolbox -- a collection of autonomous components that can be
freely mixed and matched to best meet the user's needs. The project has four
focus areas:

   -  Memory management
   -  Power management
   -  Resource management
   -  Hardware co-design

Variorum is a key player for the `power management` and `hardware co-design`
focus areas for Argo. Details about the broader Argo project, along with
relevant source code, publications, and accomplishments can be found on the
`Argo website <https://web.cels.anl.gov/projects/argo/>`_. The Argo project is
also a key contributor to the :doc:`PowerStack`, which is a community effort for
power management.

`Dynamic power management` is expected to be critical in the exascale era, both
in terms of not exceeding the overall available power budget and in terms of
utilizing the available power to make the most application progress. Argo
employs hierarchical power management that includes both system-level (global)
and node-local mechanisms and policies. This includes developing and enabling
power management in HPC schedulers (such as SLURM and Flux), large-scale system
monitoring frameworks (LDMS), application profilers (such as Caliper), runtime
system frameworks (such as Kokkos, GEOPM), and similar system- and
application-level frameworks.

`Hardware co-design` involves working closely with our vendors to continuously
explore emerging new hardware trends and devices, and look for how best to
exploit the new capabilities in both traditional HPC workloads and emerging
scientific workflows, ones such as machine learning. We try to identify how the
new features should be integrated into existing runtime systems and operating
systems for the exascale era.
