#include <inttypes.h>
#include <stdlib.h>

#include <misc_features.h>
#include <config_architecture.h>
#include <msr_core.h>
#include <variorum_error.h>

int get_max_non_turbo_ratio(off_t msr_platform_info)
{
    static int init = 0;
    static int nsockets = 0;
    static uint64_t **val = NULL;
    int max_non_turbo_ratio;

    variorum_set_topology(&nsockets, NULL, NULL);
    if (!init)
    {
        val = (uint64_t **) malloc(nsockets * sizeof(uint64_t *));
        allocate_batch(PLATFORM_INFO, nsockets);
        load_socket_batch(msr_platform_info, val, PLATFORM_INFO);
        init = 1;
    }
    read_batch(PLATFORM_INFO);

    max_non_turbo_ratio = (int)(MASK_VAL(*val[0], 15, 8));

    /// Do sockets match?
    if (nsockets != 1)
    {
        if (max_non_turbo_ratio != (int)(MASK_VAL(*val[1], 15, 8)))
        {
            return VARIORUM_ERROR_INVAL;
        }
    }
    /// 100 MHz multiplier
    return max_non_turbo_ratio * 100;
}
