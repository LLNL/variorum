#include <stdio.h>
#include <stdlib.h>

#include <config_ibm.h>
#include <config_architecture.h>
#include <Power9.h>
#include <variorum_error.h>

uint64_t *detect_ibm_arch(void)
{
    //fprintf(stdout, "Entered %s\n", __FUNCTION__);
    /* Power8 is unimplemented. This is the current hack for Power9 */
    uint64_t *model =  (uint64_t *) malloc(sizeof(uint64_t));
    *model = 9;
    return model;
}

int set_ibm_func_ptrs(void)
{
    int err = 0;
    //fprintf(stdout, "Entered %s\n", __FUNCTION__);

    if (*g_platform.ibm_arch == POWER9)
    {
        g_platform.dump_power = p9_get_power;
        g_platform.dump_power_limits = p9_get_power_limits;
        g_platform.set_node_power_limit = p9_set_power_limit;
        g_platform.set_and_verify_node_power_limit = p9_set_and_verify_power_limit;
        g_platform.monitoring = p9_monitoring;
    }
    else
    {
        return VARIORUM_ERROR_UNSUPPORTED_PLATFORM;
    }

    return err;
}
