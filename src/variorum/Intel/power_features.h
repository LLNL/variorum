#ifndef POWER_FEATURES_H_INCLUDE
#define POWER_FEATURES_H_INCLUDE

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#define UINT_MAX 4294967295U // taken from limits.h
#define STD_ENERGY_UNIT 65536.0

/// @brief Enum encompassing unit conversion types.
enum variorum_unit_conversions_e
{
    /// @brief Decode raw bits into Watts.
    BITS_TO_WATTS,
    /// @brief Encode Watt value to raw bits.
    WATTS_TO_BITS,
    /// @brief Decode raw bits into Joules.
    BITS_TO_JOULES,
    /// @brief Encode Joule value to raw bits.
    JOULES_TO_BITS,
    /// @brief Decode raw bits to seconds (for Sandy Bridge and Ivy Bridge).
    BITS_TO_SECONDS_STD,
    /// @brief Encode seconds value to raw bits (for Sandy Bridge and Ivy
    /// Bridge).
    SECONDS_TO_BITS_STD,
    /// @brief Decode raw bits to Joules for DRAM.
    BITS_TO_JOULES_DRAM
};

/// @brief Structure containing units for energy, time, and power across all
/// RAPL power domains.
struct rapl_units
{
    /// @brief Raw 64-bit value stored in MSR_RAPL_POWER_UNIT.
    uint64_t msr_rapl_power_unit;
    /// @brief Energy status units (ESU) based on the multiplier 1/(2^ESU) (in
    /// Joules). ESU is encoded in bits 12:8 of MSR_RAPL_POWER_UNIT.
    double joules;
    /// @brief Time units (TU) based on the multiplier 1/(2^TU) (in seconds).
    /// TU is encoded in bits 19:16 of MSR_RAPL_POWER_UNIT.
    double seconds;
    /// @brief Power units (PU) based on the multiplier 1/(2^PU) (in Watts). PU
    /// is encoded in bits 3:0 of MSR_RAPL_POWER_UNIT.
    double watts;
};

/// @brief Structure containing power limit data for a given RAPL power domain.
struct rapl_limit
{
    /// @brief Raw 64-bit value stored in the power limit register.
    uint64_t bits;
    /// @brief Power limit in Watts.
    double watts;
    /// @brief Time window in seconds.
    double seconds;
    /// @brief Translate bits to fields or fields to bits.
    int translate_bits;
};

/// @brief Structure containing power range info for RAPL usage for various
/// RAPL power domains.
struct rapl_power_info
{
    /**************************/
    /* RAPL Power Domain: PKG */
    /**************************/
    /// @brief Raw 64-bit value stored in MSR_PKG_POWER_INFO.
    uint64_t msr_pkg_power_info;
    /// @brief Max power (in Watts) derived from electrical specifications of
    /// the package domain.
    double pkg_max_power;
    /// @brief Min power (in Watts) derived from electrical specifications of
    /// the package domain.
    double pkg_min_power;
    /// @brief Max time (in seconds) that can be set in either time window field
    /// of the package domain.
    double pkg_max_window;
    /// @brief Thermal specification power (in Watts) of the package domain.
    double pkg_therm_power;
};

/// @brief Structure containing data from energy, time, and power measurements
/// of various RAPL power domains.
struct rapl_data
{
    /**********/
    /* Timers */
    /**********/
    /// @brief Timestamp of the current data measurement.
    struct timeval now;
    /// @brief Timestamp of the previous data measurement.
    struct timeval old_now;
    /// @brief Amount of time elapsed between the two timestamps.
    double elapsed;

    /**************************/
    /* RAPL Power Domain: PKG */
    /**************************/
    /// @brief Raw 64-bit value stored in MSR_PKG_ENERGY_STATUS.
    uint64_t **pkg_bits;
    /// @brief Raw 64-bit value previously stored in MSR_PKG_ENERGY_STATUS.
    uint64_t *old_pkg_bits;
    /// @brief Current package-level energy usage (in Joules).
    double *pkg_joules;
    /// @brief Previous package-level energy usage (in Joules).
    double *old_pkg_joules;
    /// @brief Difference in package-level energy usage between two data
    /// measurements.
    double *pkg_delta_joules;
    uint64_t *pkg_delta_bits;
    /// @brief Package-level power consumption (in Watts) derived by dividing
    /// difference in package-level energy usage by time elapsed between data
    /// measurements.
    double *pkg_watts;
    /// @brief Raw 64-bit value stored in MSR_PKG_PERF_STATUS, a package-level
    /// performance counter reporting cumulative time that the package domain
    /// has throttled due to RAPL power limits.
    uint64_t **pkg_perf_count;

    /***************************/
    /* RAPL Power Domain: DRAM */
    /***************************/
    /// @brief Raw 64-bit value stored in MSR_DRAM_ENERGY_STATUS.
    uint64_t **dram_bits;
    /// @brief Raw 64-bit value previously stored in MSR_DRAM_ENERGY_STATUS.
    uint64_t *old_dram_bits;
    /// @brief Current DRAM energy usage (in Joules).
    double *dram_joules;
    /// @brief Previous DRAM energy usage (in Joules).
    double *old_dram_joules;
    /// @brief Difference in DRAM energy usage between two data measurements.
    double *dram_delta_joules;
    /// @brief DRAM power consumption (in Watts) derived by dividing difference
    /// in DRAM energy usage by time elapsed between data measurements.
    double *dram_watts;
    /// @brief Raw 64-bit value stored in MSR_DRAM_PERF_STATUS, which counts
    /// how many times DRAM performance was capped due to underlying hardware
    /// constraints.
    uint64_t **dram_perf_count;
};

#if 0
int get_package_power_limits(struct rapl_units *ru,
                             off_t msr);
#endif

int get_rapl_power_unit(struct rapl_units *ru,
                        off_t msr);

void dump_package_power_limit(FILE *writedest,
                              off_t msr_power_limit,
                              off_t msr_rapl_unit,
                              int socket);

void dump_dram_power_limit(FILE *writedest,
                           off_t msr_power_limit,
                           off_t msr_rapl_unit,
                           int socket);

void print_dram_power_limit(FILE *writedest,
                            off_t msr_power_limit,
                            off_t msr_rapl_unit,
                            int socket);

int get_package_rapl_limit(const unsigned socket,
                           struct rapl_limit *limit1,
                           struct rapl_limit *limit2,
                           off_t msr_power_limit,
                           off_t msr_rapl_unit);

int get_dram_rapl_limit(const unsigned socket,
                        struct rapl_limit *limit,
                        off_t msr_power_limit,
                        off_t msr_rapl_unit);

void dump_rapl_power_unit(FILE *writedest,
                          off_t msr);

int get_rapl_power_info(const unsigned socket,
                        struct rapl_power_info *info,
                        off_t msr);

void dump_package_power_info(FILE *writedest,
                             off_t msr,
                             int socket);

void print_rapl_power_unit(FILE *writedest,
                           off_t msr);

void print_package_power_limit(FILE *writedest,
                               off_t msr_power_limit,
                               off_t msr_rapl_unit,
                               int socket);

void print_package_power_info(FILE *writedest,
                              off_t msr,
                              int socket);

int set_package_power_limit(const unsigned socket,
                            int package_power_limit,
                            off_t msr_power_limit,
                            off_t msr_rapl_unit);

void dump_power_data(FILE *writedest,
                     off_t msr_power_limit,
                     off_t msr_rapl_unit,
                     off_t msr_pkg_energy_status,
                     off_t msr_dram_energy_status);

void print_power_data(FILE *writedest,
                      off_t msr_power_limit,
                      off_t msr_rapl_unit,
                      off_t msr_pkg_energy_status,
                      off_t msr_dram_energy_status);

/// @brief Store the RAPL data on the heap.
///
/// @param [out] data Pointer to measurements of energy, time, and power data
///        from a given RAPL power domain.
///
/// @return 0 if successful, else -1 if setflags() fails.
int rapl_storage(struct rapl_data **data);

/// @brief Read all available RAPL data for a given socket.
///
/// @param [in] msr_rapl_unit Unique MSR address for MSR_RAPL_POWER_UNIT.
/// @param [in] msr_pkg_energy_status Unique MSR address for MSR_PKG_ENERGY_STATUS.
/// @param [in] msr_dram_energy_status Unique MSR address for MSR_DRAM_ENERGY_STATUS.
///
/// @return 0 if successful, else -1 if rapl_storage() fails.
int read_rapl_data(off_t msr_rapl_unit,
                   off_t msr_pkg_energy_status,
                   off_t msr_dram_energy_status);

/// @brief Read RAPL data and compute difference in readings taken at two
/// instances in time.
///
/// @param [in] msr_rapl_unit Unique MSR address for MSR_RAPL_POWER_UNIT.
/// @param [in] msr_pkg_energy_status Unique MSR address for MSR_PKG_ENERGY_STATUS.
/// @param [in] msr_dram_energy_status Unique MSR address for MSR_DRAM_ENERGY_STATUS.
///
/// @return 0 if successful, else -1 if rapl_storage() fails.
int get_power(off_t msr_rapl_unit,
              off_t msr_pkg_energy_status,
              off_t msr_dram_energy_status);

void get_all_power_data(FILE *writedest,
                        off_t msr_pkg_power_limit,
                        off_t msr_dram_power_limit,
                        off_t msr_rapl_unit,
                        off_t msr_package_energy_status,
                        off_t msr_dram_energy_status);

/// @brief Compute difference in readings taken at two instances in time.
///
/// @return 0 if successful, else -1 if rapl_storage() fails.
int delta_rapl_data(off_t msr_rapl_unit);

#endif

///* power_features.h */
//void pkg_energy_status(enum ctl_domains_e d,
//                       int id,
//                       off_t msr,
//                       double *val);
//void dram_energy_status(enum ctl_domains_e d,
//                        int id,
//                        off_t msr,
//                        double *val);
