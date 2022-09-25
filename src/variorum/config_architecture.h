// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#ifndef CONFIG_ARCHIECTURE_H_INCLUDE
#define CONFIG_ARCHIECTURE_H_INCLUDE

#include <stdint.h>

#include <variorum_config.h>

/// @brief Create a mask from bit m to n (63 >= m >= n >= 0).
///
/// Example: MASK_RANGE(4,2) --> (((1<<((4)-(2)+1))-1)<<(2))
///                              (((1<<          3)-1)<<(2))
///                              ((               4-1)<<(2))
///                              (                  3)<<(2))
///                              (                       24) = b11000
#define MASK_RANGE(m,n) ((((uint64_t)1<<((m)-(n)+1))-1)<<(n))

/// @brief Return the value of x after applying bitmask (m,n) (63 >= m >= n >= 0).
///
/// Example: MASK_RANGE(17,4,2) --> 17&24 = b10001 & b11000 = b10000
#define MASK_VAL(x,m,n) (((uint64_t)(x)&MASK_RANGE((m),(n)))>>(n))

/// @brief List of different granularities of control.
enum ctl_domains_e
{
    THREAD,
    CORE,
    SOCKET,
    NODE,
    TILE,
    UNIQUE, // Each processor core has a separate MSR,
    // or a bit field in an MSR governs only a core independently.
    SHARED // MSR or bit field that governs the operation of both processor cores.
};

/// @brief List of Intel family and models.
enum intel_arch_e
{
    FM_06_2A = 0x2A, //Sandy Bridge
    FM_06_2D = 0x2D, //Sandy Bridge
    FM_06_3A = 0x3A, //Ivy Bridge
    FM_06_3C = 0x3C, //Haswell
    FM_06_3D = 0x3D, //Broadwell
    FM_06_3E = 0x3E, //Ivy Bridge
    FM_06_3F = 0x3F, //Haswell
    FM_06_45 = 0x45, //Haswell
    FM_06_46 = 0x46, //Haswell
    FM_06_47 = 0x47, //Broadwell
    FM_06_4E = 0x4E, //Skylake
    FM_06_4F = 0x4F, //Broadwell
    FM_06_55 = 0x55, //Skylake, Cascade Lake, Cooper Lake
    FM_06_56 = 0x56, //Broadwell
    FM_06_5E = 0x5E, //Skylake
    FM_06_66 = 0x66, //Cannon Lake
    FM_06_6A = 0x6A, //Ice Lake
    FM_06_6C = 0x6C, //Ice Lake
    FM_06_7D = 0x7D, //Ice Lake
    FM_06_7E = 0x7E, //Ice Lake
    FM_06_8C = 0x8C, //Tiger Lake
    FM_06_8D = 0x8D, //Tiger Lake
    FM_06_8E = 0x8E, //Coffee Lake
    FM_06_8F = 0x8F, //Sapphire Rapids
    FM_06_97 = 0x97, //Alder Lake
    FM_06_9E = 0x9E, //Kaby Lake, Coffee Lake
    FM_06_9A = 0x9A, //Alder Lake
    FM_06_A5 = 0xA5, //Comet Lake
    FM_06_A6 = 0xA6, //Comet Lake
    FM_06_A7 = 0xA7, //Rocket Lake (No MSRs listed in SDM)
    FM_06_A8 = 0xA8, //Rocket Lake (No MSRs listed in SDM)
    FM_06_BF = 0xBF, //Alder Lake
};

/// @brief List of IBM family and models.
/// @todo: we need to find architecture detection info here.
enum ibm_arch_e
{
    POWER9 = 9, // Power9
};

/// @brief List of NVIDIA family and models.
/// @todo: we need to find architecture detection info here.
enum nvidia_gpu_arch_e
{
    VOLTA = 1, // Volta
};

enum arm_arch_e
{
    ARMV8 = 1, //ARM Juno
};

/// @brief List of AMD GPU family and models.
enum amd_gpu_arch_e
{
    AMD_INSTINCT = 1,
};

/// @brief Platform-specific information.
///
/// The intersection of all features on all platforms.
struct platform
{
    int (*variorum_poll_power)(FILE *output);

    int (*variorum_monitoring)(FILE *output);

    /***********************************/
    /* Feature-Based Function Pointers */
    /***********************************/
    /// @brief Function pointer to print out power limits.
    ///
    /// @param [in] long_ver Toggle between CSV formatted and long formatted
    ///        output.
    ///
    /// @return Error code.
    int (*variorum_print_power_limit)(int long_ver);

    /// @brief Function pointer to set a power limit on the node.
    ///
    /// @param [in] node_power_limit Desired node power limit in Watts.
    ///
    /// @return Error code.
    int (*variorum_cap_best_effort_node_power_limit)(int node_power_limit);

    /// @brief Function pointer to set socket frequency
    ///
    /// @param [in] chipid Socket ID.
    /// @param [in] socket_frequency Desired socket frequency in Hertz.
    ///
    /// @return Error code.
    int (*variorum_cap_socket_frequency_limit)(int chipid, int socket_frequency);

    /// @brief Set the GPU power shifting ratio (uniform across sockets).
    ///
    /// @param [in] gpu_power_ratio Desired power ratio (percent) for the
    ///        processor and GPU.
    ///
    /// @note Only valid on IBM P9 systems for now. Same ratio will be set on
    /// both sockets.
    ///
    /// @return Error code.
    int (*variorum_cap_gpu_power_ratio)(int gpu_power_ratio);

    /// @brief Function pointer to set a power limit to each socket.
    ///
    /// @param [in] socket_power_limit Desired socket power limit in Watts.
    ///
    /// @return Error code.
    int (*variorum_cap_each_socket_power_limit)(int socket_power_limit);

    int (*variorum_cap_each_core_frequency_limit)(int core_freq_mhz);

    /// @brief Function pointer to print the feature set.
    ///
    /// @return Error code.
    int (*variorum_print_features)(void);

    /// @brief Function pointer to print out thermal data.
    ///
    /// @param [in] long_ver Toggle between CSV formatted and long formatted
    ///        output.
    ///
    /// @return Error code.
    int (*variorum_print_thermals)(int long_ver);

    /// @brief Function pointer to print out counter data.
    ///
    /// @param [in] long_ver Toggle between CSV formatted and long formatted
    ///        output.
    ///
    /// @return Error code.
    int (*variorum_print_counters)(int long_ver);

    /// @brief Function pointer to print out clocks data.
    ///
    /// @param [in] long_ver Toggle between CSV formatted and long formatted
    ///        output.
    ///
    /// @return Error code.
    int (*variorum_print_frequency)(int long_ver);

    /// @brief Function pointer to print out power consumption data.
    ///
    /// @param [in] long_ver Toggle between CSV formatted and long formatted
    ///        output.
    ///
    /// @return Error code.
    int (*variorum_print_power)(int long_ver);

    /// @brief Function pointer to enable turbo.
    ///
    /// @return Error code.
    int (*variorum_enable_turbo)(void);

    /// @brief Function pointer to disable turbo.
    ///
    /// @return Error code.
    int (*variorum_disable_turbo)(void);

    /// @brief Function pointer to get print turbo data.
    ///
    /// @return Error code.
    int (*variorum_print_turbo)(void);

    /// @brief Function pointer to print GPU utilization.
    ///
    /// @return Error code.
    int (*variorum_print_gpu_utilization)(int long_ver);

    /// @brief Function pointer to get JSON object for node power data.
    ///
    /// @return Error code.
    int (*variorum_get_node_power_json)(char **get_power_obj_str);

    /// @brief Function pointer to get JSON object for power domain information.
    ///
    /// @return Error code.
    int (*variorum_get_node_power_domain_info_json)(char **get_domain_obj_str);

    /// @brief Function pointer to get list of available frequencies.
    ///
    /// @return Error code.
    int (*variorum_print_available_frequencies)(void);

    /// @brief Function pointer to print energy for all cores and sockets.
    ///
    /// @return Error code.
    int (*variorum_print_energy)(void);

    /******************************/
    /* Platform-Specific Topology */
    /******************************/
    /// @brief Unique family and model for Intel CPU architectures.
    uint64_t *intel_arch;
    /// @brief Unique family and model for Intel GPU architectures.
    uint64_t *intel_gpu_arch;
    /// @brief Identifier for AMD CPU architecture.
    uint64_t *amd_arch;
    /// @brief Identifier for IBM CPU architecture.
    uint64_t *ibm_arch;
    /// @brief Identifier for NVIDIA GPU architecture.
    uint64_t *nvidia_gpu_arch;
    /// @brief Identifier for ARM CPU architecture.
    uint64_t *arm_arch;
    /// @brief Identifier for AMD GPU architecture.
    uint64_t *amd_gpu_arch;

    /// @brief Hostname.
    char hostname[1024];
    /// @brief Number of sockets in the node.
    int num_sockets;
    /// @brief Total number of physical cores in the node.
    int total_cores;
    /// @brief Total number of logical threads in the node.
    int total_threads;
    /// @brief Number of physical cores per socket in the node.
    int num_cores_per_socket;
    /// @brief Number of logical threads per core.
    int num_threads_per_core;
};

#if 0 /* To implement later */
//    void (*set_fixed_counters)();
//    void (*get_performance_counters)();
//    void (*set_performance_counters)();
#endif

extern struct platform g_platform;

int variorum_enter(const char *filename,
                   const char *func_name,
                   int line_num);

int variorum_exit(const char *filename,
                  const char *func_name,
                  int line_num);

void variorum_get_topology(unsigned *nsockets,
                           unsigned *ncores,
                           unsigned *nthreads);

int variorum_set_func_ptrs(void);

int variorum_detect_arch(void);

void variorum_init_func_ptrs(void);

#endif
