// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <config_amd.h>
#include <config_architecture.h>
#include <epyc.h>
#include <variorum_error.h>

uint64_t *detect_amd_arch(void)
{
    uint64_t rax = 1;
    uint64_t rbx = 0;
    uint64_t rcx = 0;
    uint64_t rdx = 0;
    uint64_t *fh_model = (uint64_t *) malloc(sizeof(uint64_t));

    asm volatile(
        "cpuid"
        : "=a"(rax), "=b"(rbx), "=c"(rcx), "=d"(rdx)
        : "0"(rax), "2"(rcx));

    /*
     * Functionality will depand on both Family and Model
     * Hence return value for this function contains both Family and Model
     * Family is the [16:8] bit and Model is [7:0] bit in return value
     */
    *fh_model = (((((rax >> 8) & 0xf) + ((rax >> 20) & 0xff)) << 8) |
                (((rax >> 16) & 0xf) * 0x10 + ((rax >> 4) & 0xf))) & 0xFFFF;
    return fh_model;
}

int set_amd_func_ptrs(void)
{
    int ret = 0;
    uint8_t family, model;
    family = (*g_platform.amd_arch >> 8) & 0xFF;
    model = *g_platform.amd_arch & 0xFF;

    /* Verify for the family and model */
    if (family == 0x19)
    {
        switch(model)
        {
        case 0x0 ... 0xF:
        case 0x30 ... 0x3F:
            break;
        default:
            return VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
        }
    }
    else
        return VARIORUM_ERROR_UNSUPPORTED_PLATFORM;

    /* smi monitor initialization */
    ret = esmi_init();
    switch (ret)
    {
    case 0:
        g_platform.variorum_print_power = epyc_get_power;
        g_platform.variorum_print_power_limits = epyc_get_power_limits;
        g_platform.variorum_cap_each_socket_power_limit = epyc_set_socket_power_limit;
        g_platform.variorum_cap_and_verify_node_power_limit = epyc_set_and_verify_node_power_limit;
        g_platform.variorum_print_energy = epyc_print_energy;
        g_platform.variorum_print_boostlimit = epyc_print_boostlimit;
        g_platform.variorum_set_and_verify_core_boostlimit = epyc_set_and_verify_core_boostlimit;
        g_platform.variorum_set_socket_boostlimit = epyc_set_socket_boostlimit;
        break;
    default:
        fprintf(stdout, "ESMI not initialized, drivers not found. "
                "Msg[%d]: %s\n", ret, esmi_get_err_msg(ret));
        g_platform.variorum_print_energy = epyc_print_energy;
        ret = 0;
    }
    return ret;
}
