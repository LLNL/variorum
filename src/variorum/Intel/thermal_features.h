// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef THERMAL_FEATURES_H_INCLUDE
#define THERMAL_FEATURES_H_INCLUDE

/// @brief Structure containing data from MSR_TEMPERATURE_TARGET.
///
/// The scope of this MSR is defined as unique for Sandy Bridge. In our
/// implementation, we assume a socket-level scope.
struct msr_temp_target
{
    /// @brief Raw 64-bit value stored in MSR_TEMPERATURE_TARGET.
    uint64_t raw;
    /// @brief Min temperature (in degree Celsius) at which PROCHOT will be
    /// asserted.
    ///
    /// This bit field (bits 23:16) is RO and is likely equal to the TCC
    /// Activation Temperature.
    uint64_t temp_target;
};

/// @brief Structure holding data for IA32_THERM_STATUS.
///
/// The scope of this MSR is core-level for Sandy Bridge.
struct therm_stat
{
    /// @brief Raw 64-bit value stored in IA32_THERM_STATUS.
    uint64_t raw;
    /// @brief Status (active/not active) of the digital thermal sensor
    /// high-temperature output signal (PROCHOT#) for the core.
    ///
    /// The thermal status bit indicates whether the digital thermal sensor
    /// high-temperature output signal (PROCHOT#) is currently active. If 1,
    /// PROCHOT# is active, else the feature is not active. This bit field (bit
    /// 0) is RO.
    int status;
    /// @brief Sticky bit indicating history of the thermal sensor
    /// high-temperature output signal (PROCHOT#) since last cleared or last
    /// reset.
    ///
    /// Sticky bit indicating history of the thermal sensor high temperature
    /// output signal (PROCHOT#). If 1, PROCHOT# has been asserted since a
    /// previous RESET or the last time software cleared the bit. This bit (bit
    /// 1) is R/W, but software can only clear the bit by writing a 0.
    int status_log;
    /// @brief Indicates if PROCHOT# or FORCEPR# is being asserted by another
    /// component on the platform.
    ///
    /// Indicates whether another component on the platform is causing
    /// high-temperature and asserting PROCHOT# or FORCEPR# as a result. This
    /// bit field (bit 2) is RO.
    int PROCHOT_or_FORCEPR_event;
    /// @brief Sticky bit indicating history of the PROCHOT# and FORCEPR#
    /// signals since last cleared or last reset.
    ///
    /// Sticky bit indicating history of the PROCHOT# and FORCEPR# signals. If
    /// 1, PROCHOT# or FORCEPR# has been externally asserted by another agent
    /// on the platform since the last clearing of this bit or a reset.
    /// External PROCHOT# assertions are only acknowledged if the Bidirectional
    /// Prochot feature is enabled. This bit (bit 3) is R/W, but software can
    /// only clear the bit by writing a 0.
    int PROCHOT_or_FORCEPR_log;
    /// @brief Status (active/not active) of the critical temperature detector
    /// output signal.
    ///
    /// Indicates whether the critical temperature detector output signal is
    /// currently active. If 1, the critical temperature detector output
    /// signal is currently active, else it is not active. This bit (bit 4) is
    /// RO.
    int crit_temp_status;
    /// @brief Sticky bit indicating history of the critical temperature
    /// detector output signal since last cleared or last reset.
    ///
    /// Sticky bit indicating history of the critical temperature detector
    /// output signal. If 1, the signal has been asserted since a previous
    /// RESET or the last time software cleared the bit. This bit (bit 5) is
    /// R/W, but software can only clear the bit by writing a 0.
    int crit_temp_log;
    /// @brief Indicates if the actual core temperature is currently higher
    /// than or equal to Thermal Threshold #1.
    ///
    /// Indicates whether the actual core temperature is currently higher than
    /// or equal to the value set in Thermal Threshold #1. If 0, the actual
    /// temperature is lower, else the actual core temperature is greater than
    /// or equal to TT#1. This bit field (bit 6) is RO.
    int therm_thresh1_status;
    /// @brief Sticky bit indicating history of Thermal Threshold #1 (TT#1)
    /// since last cleared or last reset.
    ///
    /// Sticky bit indicating whether the Thermal Threshold #1 (TT#1) has been
    /// reached since the last clearing of this bit or a reset. If 1, TT#1 has
    /// been reached. This bit field (bit 7) is R/W, but software can only
    /// clear the bit by writing a 0.
    int therm_thresh1_log;
    /// @brief Indicates if the actual temperature is currently higher than or
    /// equal to Thermal Threshold #2 (TT#2).
    ///
    /// Indicates whether the actual temperature is currently higher than or
    /// equal to the value set in Thermal Threshold #2. If 0, the actual
    /// temperature is lower, else the actual temperature is greater than or
    /// equal to TT#2. This bit field (bit 8) is RO.
    int therm_thresh2_status;
    /// @brief Sticky bit indicating history of Thermal Threshold #2 (TT#2)
    /// since last cleared or last reset.
    ///
    /// Sticky bit indicating whether the Thermal Threshold #2 (TT#2) has been
    /// reached since the last clearing of this bit or a reset. If 1, TT#2 has
    /// been reached. This bit field (bit 9) is R/W, but software can only
    /// clear the bit by writing a 0.
    int therm_thresh2_log;
    /// @brief Indicates if processor is operating below OS-requested p-state or
    /// OS-requested clock modulation duty cycle.
    ///
    /// Indicates whether the processor is currently operating below
    /// OS-requested p-state (specified in IA32_PERF_CTL) or OS-requested clock
    /// modulation duty cycle (specified in IA32_CLOCK_MODULATION). This bit
    /// (bit 10) is RO.
    int power_limit_status;
    /// @brief Sticky bit indicating if processor went below OS-requested
    /// p-state or OS-requested clock modulation duty cycle since last cleared
    /// or last reset.
    ///
    /// Sticky bit indicating whether the processor went below the OS-requested
    /// p-state or OS-requested clock modulation duty cycle since the last
    /// clearing of this bit or a reset. This informs software of such
    /// occurrence. If 1, processor went below OS requests. This bit field (bit 11)
    /// is R/W, but software can only clear the bit by writing a 0.
    int power_notification_log;
    /// @brief Digital temperature reading in degree Celsius relative to the
    /// TCC activation temperature.
    ///
    /// This bit field (bits 22:16) is RO.
    int readout;
    /// @brief Specifies the resolution (or tolerance) of the digital thermal
    /// sensor in degree Celsius.
    ///
    /// This bit field (bits 30:27) is RO.
    int resolution_deg_celsius;
    /// @brief Indicates if digital readout (bits 22:16) is valid.
    ///
    /// If 1, digital readout is valid, else it is not valid. This bit field
    /// (bit 31) is RO.
    int readout_valid;
};

/// @brief Structure holding data for IA32_PACKAGE_THERM_STATUS.
///
/// The scope of this MSR is package-level for Sandy Bridge.
struct pkg_therm_stat
{
    /// @brief Raw 64-bit value stored in IA32_PACKAGE_THERM_STATUS.
    uint64_t raw;
    /// @brief Status (active/not active) of the digital thermal sensor
    /// high-temperature output signal (PROCHOT#) for the package.
    ///
    /// The thermal status bit indicates whether the digital thermal sensor
    /// high-temperature output signal (package PROCHOT#) is currently active.
    /// If 1, package PROCHOT# is active, else the feature is not active. This
    /// bit field (bit 0) is RO.
    int status;
    /// @brief Sticky bit indicating history of the package thermal sensor
    /// high-temperature output signal (PROCHOT#) since last cleared or last
    /// reset.
    ///
    /// Sticky bit indicating history of the package thermal sensor high
    /// temperature output signal (package PROCHOT#). If 1, package PROCHOT#
    /// has been asserted since a previous RESET or the last time software
    /// cleared the bit. This bit (bit 1) is R/W, but software can only clear
    /// the bit by writing a 0.
    int status_log;
    /// @brief Indicates if package PROCHOT# # is being asserted by another
    /// component on the platform.
    ///
    /// Indicates whether another component on the platform is causing
    /// high-temperature and asserting package PROCHOT# as a result. This bit
    /// field (bit 2) is RO.
    int PROCHOT_event;
    /// @brief Sticky bit indicating history of the package PROCHOT# signal
    /// since last cleared or last reset.
    ///
    /// Sticky bit indicating history of the package PROCHOT# signal. If 1
    /// 1, package PROCHOT# has been externally asserted by another agent on
    /// the platform since the last clearing of this bit or a reset. This bit
    /// (bit 3) is R/W, but software can only clear the bit by writing a 0.
    int PROCHOT_log;
    /// @brief Status (active/not active) of the package critical temperature
    /// detector output signal.
    ///
    /// Indicates whether the package critical temperature detector output
    /// signal is currently active. If 1, the critical temperature detector output
    /// signal is currently active, else it is not active. This bit (bit 4) is
    /// RO.
    int crit_temp_status;
    /// @brief Sticky bit indicating history of the package critical temperature
    /// detector output signal since last cleared or last reset.
    ///
    /// Sticky bit indicating history of the package critical temperature
    /// detector output signal. If 1, the signal has been asserted since a
    /// previous RESET or the last time software cleared the bit. This bit (bit
    /// 5) is R/W, but software can only clear the bit by writing a 0.
    int crit_temp_log;
    /// @brief Indicates if the actual package temperature is currently higher
    /// than or equal to Package Thermal Threshold #1 (PTT#1).
    ///
    /// Indicates whether the actual package temperature is currently higher
    /// than or equal to the value set in Package Thermal Threshold #1 (PTT#1).
    /// If 0, the actual temperature is lower, else the actual temperature is
    /// greater than or equal to PTT#1. This bit field (bit 6) is RO.
    int therm_thresh1_status;
    /// @brief Sticky bit indicating history of Package Thermal Threshold #1
    /// (PTT#1) since last cleared or last reset.
    ///
    /// Sticky bit indicating whether the Package Thermal Threshold #1 (PTT#1)
    /// has been reached since the last clearing of this bit or a reset. If 1,
    /// PTT#1 has been reached. This bit field (bit 7) is R/W, but software
    /// can only clear the bit by writing a 0.
    int therm_thresh1_log;
    /// @brief Indicates if the actual package temperature is currently higher
    /// than or equal to Package Thermal Threshold #2 (PTT#2).
    ///
    /// Indicates whether the actual package temperature is currently higher
    /// than or equal to the value set in Package Thermal Threshold #2 (PTT#2).
    /// If 0, the actual temperature is lower, else the actual temperature is
    /// greater than or equal to PTT#2. This bit field (bit 8) is RO.
    int therm_thresh2_status;
    /// @brief Sticky bit indicating history of Package Thermal Threshold #2
    /// (PTT#2) since last cleared or last reset.
    ///
    /// Sticky bit indicating whether the Package Thermal Threshold #2 (PTT#2)
    /// has been reached since the last clearing of this bit or a reset. If 1,
    /// PTT#2 has been reached. This bit field (bit 9) is R/W, but software
    /// can only clear the bit by writing a 0.
    int therm_thresh2_log;
    /// @brief Indicates package power limit is forcing at least one processor
    /// to operate below OS-requested p-state.
    ///
    /// Indicates if package power limit is forcing one or more processors to
    /// operate below OS-requested p-state. Software can examine
    /// IA32_THERM_STATUS to determine if the cause originates from a processor
    /// core. This bit (bit 10) is RO.
    int power_limit_status;	//Read only
    /// @brief Sticky bit indicating if any processor in the package went below
    /// OS-requested p-state or OS-requested clock modulation duty cycle since
    /// last cleared or last reset.
    ///
    /// Sticky bit indicating whether any processor in the package went below
    /// the OS-requested p-state or OS-requested clock modulation duty cycle
    /// since the last clearing of this bit or a reset. This informs software
    /// of such occurrence. If 1, processor went below OS requests. This bit
    /// field (bit 11) is R/W, but software can only clear the bit by writing a
    /// 0.
    int power_notification_log;
    /// @brief Package digital temperature reading in degree Celsius relative
    /// to the TCC activation temperature.
    ///
    /// This bit field (bits 22:16) is RO.
    int readout;
};

/// @brief Print only temperature data in CSV format.
///
/// @param [in] writedest File stream where output will be written to.
/// @param [in] msr_therm_stat Unique MSR address for IA32_THERM_STATUS.
/// @param [in] msr_pkg_therm_stat Unique MSR address for IA32_PACKAGE_THERM_STATUS.
/// @param [in] msr_temp_target Unique MSR address for TEMPERATURE_TARGET.
int dump_therm_temp_reading(FILE *writedest,
                            off_t msr_therm_stat,
                            off_t msr_pkg_therm_stat,
                            off_t msr_temp_target);

/// @brief Print only temperature data in long format.
///
/// @param [in] writedest File stream where output will be written to.
/// @param [in] msr_therm_stat Unique MSR address for IA32_THERM_STATUS.
/// @param [in] msr_pkg_therm_stat Unique MSR address for IA32_PACKAGE_THERM_STATUS.
/// @param [in] msr_temp_target Unique MSR address for TEMPERATURE_TARGET.
int print_therm_temp_reading(FILE *writedest,
                             off_t msr_therm_stat,
                             off_t msr_pkg_therm_stat,
                             off_t msr_temp_target);

/// @brief Read value of the IA32_PACKAGE_THERM_STATUS register and translate
/// bit fields to human-readable values.
///
/// @param [out] s Data for package-level thermal status.
/// @param [in] msr Unique MSR address for IA32_PACKAGE_THERM_STATUS.
int get_pkg_therm_stat(struct pkg_therm_stat *s,
                       off_t msr);

/************************************/
/* MSR_TEMPERATURE_TARGET Functions */
/************************************/

/// @brief Retrieve current minimum temperature (in degree Celsius) at which
/// PROCHOT will be asserted.
///
/// @param [out] s Data for target temperature.
/// @param [in] msr Unique MSR address for TEMPERATURE_TARGET.
void get_temp_target(struct msr_temp_target *s,
                     off_t msr);

/*********************************************/
/* Thermal Functions (Status and Interrupts) */
/*********************************************/

/// @brief Read value of the IA32_THERM_STATUS register and translate bit
/// fields to human-readable values.
///
/// @param [out] s Data for per-core thermal status.
/// @param [in] msr Unique MSR address for IA32_THERM_STATUS.
void get_therm_stat(struct therm_stat *s,
                    off_t msr);

///// @brief Store the package-level thermal control data on the heap.
/////
///// @param [out] thermctlref Pointer to data for package-level thermal control.
/////
///// @return 0 if successful.
//int therm2_ctl_storage(uint64_t **thermctlref);
//
///// @brief Read value of the MSR_THERM2_CTL register.
/////
///// @return 0 if successful, else 1 if read_msr_by_coord() fails.
//int get_pkg_therm2_ctl(void);
//
///// @brief Print out control settings for Thermal Monitor #2.
/////
///// @param [in] writedest File stream where output will be written to.
/////
///// @return 0 if successful, else return a value less than 0 if
///// therm2_ctl_storage() fails.
//int dump_therm2_ctl(FILE *writedest);
//
///// @brief Set value for IA32_THERM_STATUS across all cores.
/////
///// @param [in] s Data for per-core thermal status.
//void set_therm_stat(struct therm_stat *s);
//
///// @brief Set value for IA32_PACKAGE_THERM_STATUS across all sockets.
/////
///// @param [in] s Data for package-level thermal status.
//void set_pkg_therm_stat(struct pkg_therm_stat *s);
//
///// @brief Print detailed thermal status and interrupt data.
/////
///// @param [in] writedest File stream where output will be written to.
//void dump_therm_data_verbose(FILE *writedest);
//
///// @brief Store the per-core thermal interrupt data on the heap.
/////
///// @param [out] ti Pointer to data for per-core thermal interrupts.
//void store_therm_interrupt(struct therm_interrupt **ti);
//
///// @brief Store the package-level thermal interrupt data on the heap.
/////
///// @param [out] pi Pointer to data for package-level thermal interrupts.
//void store_pkg_therm_interrupt(struct pkg_therm_interrupt **pi);
//
///// @brief Read value of the IA32_THERM_INTERRUPT register and translate bit
///// fields to human-readable values.
/////
///// @param [out] s Data for per-core thermal interrupts.
//void get_therm_interrupt(struct therm_interrupt *s);
//
///// @brief Read value of the IA32_PACKAGE_THERM_INTERRUPT register and
///// translate bit fields to human-readable values.
/////
///// @param [out] s Data for package-level thermal interrupts.
//void get_pkg_therm_interrupt(struct pkg_therm_interrupt *s);
//
///// @brief Set value for IA32_THERM_INTERRUPT across all cores.
/////
///// @param [in] s Data for per-core thermal interrupts.
//void set_therm_interrupt(struct therm_interrupt *s);
//
///// @brief Set value for IA32_PACKAGE_THERM_INTERRUPT across all sockets.
/////
///// @param [in] s Data for package-level thermal interrupts.
//void set_pkg_therm_interrupt(struct pkg_therm_interrupt *s);
//
///// @brief Print the label for the abbreviated thermal status and interrupt
///// data printout.
/////
///// @param [in] writedest File stream where output will be written to.
//void dump_therm_data_terse_label(FILE *writedest);
//
///// @brief Print abbreviated thermal status and interrupt data.
/////
///// @param [in] writedest File stream where output will be written to.
//void dump_therm_data_terse(FILE *writedest);
//
///// @brief Print the label for the detailed thermal status and interrupt data
///// printout.
/////
///// @param [in] writedest File stream where output will be written to.
//void dump_therm_data_verbose_label(FILE *writedest);

#endif
