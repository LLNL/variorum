// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef VARIORUM_TIMERS_H_INCLUDE
#define VARIORUM_TIMERS_H_INCLUDE

struct mstimer
{
    /// @brief When we started tracking the timer.
    unsigned long startms;
    /// @brief Which time is the next interval.
    unsigned int step;
    /// @brief How many ms between firings.
    unsigned int interval;
    /// @brief When does the timer expire next.
    unsigned long nextms;
};

/// @brief Get a number of millis from a monotonic clock.
unsigned long now_ms(void);

/// @brief Sleep until timer time has elapsed.
int timer_sleep(struct mstimer *t);

/// @brief Initialize a msTimer.
void init_msTimer(struct mstimer *t,
                  int ms_interval);

/// @brief Use a select to sleep a given number of millis.
void sleep_ms(long ms);

#endif
