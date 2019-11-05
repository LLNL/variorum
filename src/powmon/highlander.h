// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef HIGHLANDER_H
#define HIGHLANDER_H

/// @brief Determines/initializes the process highlander status.
int highlander(void);

/// @brief Causes the highlander to wait until all foes have called wait.
int highlander_wait(void);

/// @brief Remove outstanding semaphores.
int highlander_clean(void);

#endif
