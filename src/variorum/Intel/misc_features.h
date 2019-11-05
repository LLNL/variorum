// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef MISC_FEATURES_H_INCLUDE
#define MISC_FEATURES_H_INCLUDE

#include <stdio.h>

/// @brief Read the maximum non-turbo ratio, which is the ratio of the
/// frequency that invariant TSC runs at.
///
/// @param [in] msr_platform_info Unique MSR address for MSR_PLATFORM_INFO.
///
/// @return Maximum non-turbo ratio in Hz.
int get_max_non_turbo_ratio(off_t msr_platform_info);

/// NOTE about Turbo bit -- Pulled from the Intel Vol. 4 2-154 Documentation
/// When set to 1 on processors that support Intel Turbo Boost Technology,
/// the turbo mode feature is disabled and the IDA_Enable feature flag will
/// be clear (CPUID.06H: EAX[1]=0).

/// @brief Enable Turbo Mode on each socket.
///
/// @param [in] msr_misc_enable Unique MSR address for IA32_MISC_ENABLE.
/// @param [in] turbo_mode_disable_bit Specific bit field in IA32_MISC_ENABLE
///             register for disabling turbo mode.
///
/// @return 0 if successful, else non-zero.
int set_turbo_on(off_t msr_misc_enable,
                 unsigned int turbo_mode_disable_bit);

/// @brief Disable Turbo Mode on each socket.
///
/// @param [in] msr_misc_enable Unique MSR address for IA32_MISC_ENABLE.
/// @param [in] turbo_mode_disable_bit Specific bit field in IA32_MISC_ENABLE
///             register for disabling turbo mode.
///
/// @return 0 if successful, else non-zero.
int set_turbo_off(off_t msr_misc_enable,
                  unsigned int turbo_mode_disable_bit);

/// @brief Print status of Turbo Mode on each socket.
///
/// @param [in] writedest File stream where output will be written to
/// @param [in] msr_misc_enable Unique MSR address for IA32_MISC_ENABLE.
/// @param [in] turbo_mode_disable_bit Specific bit field in IA32_MISC_ENABLE
///             register for disabling turbo mode.
///
/// @return 0 if successful, else non-zero.
int dump_turbo_status(FILE *writedest,
                      off_t msr_misc_enable,
                      unsigned int turbo_mode_disable_bit);


///// These per core functions seemingly only for Intel Signatures 06_57H (KNL) and
///// 06_85H (future Xeon Phi), at the moment.
///// Intel Vol. 4 2-287 Documentation
//int set_turbo_on_core(unsigned const socket,
//                      unsigned const core,
//                      off_t msr_misc_enable,
//                      unsigned int turbo_mode_disable_bit);
//int set_turbo_off_core(unsigned const socket,
//                       unsigned const core,
//                       off_t msr_misc_enable,
//                       unsigned int turbo_mode_disable_bit);
///// @brief
///// @return -1 if there is an error, 0 if Turbo is DISabled, 1 if Turbo is
///// ENabled.
//int get_turbo_status_core(unsigned const socket,
//                          unsigned const core,
//                          off_t msr_misc_enable,
//                          unsigned int turbo_mode_disable_bit);

#endif
