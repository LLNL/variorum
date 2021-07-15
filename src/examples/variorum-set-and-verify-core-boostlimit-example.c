// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;
    int core;
    unsigned int boostlimit = 0;

    printf("Usage:%s <core> <boostlimit>\n", argv[0]);

    if (argc == 1)
    {
        printf("CPU number not specified...using default cpu 0\n");
        core = 0;
        printf("No boostlimit specified...using default limit of 400MHz.\n");
        boostlimit = 400;
    }
    else if (argc == 2)
    {
        core = atoi(argv[1]);
        printf("Specified core num : %d.\n", core);
        printf("No boostlimit specified...using default limit of 400MHz.\n");
        boostlimit = 400;
    }
    else if (argc > 2)
    {
        core = atoi(argv[1]);
        boostlimit = atoi(argv[2]);
        printf("Setting boostlimit for core[%d] to %u(MHz).\n", core, boostlimit);
    }

    ret = variorum_set_and_verify_core_boostlimit(core, boostlimit);
    if (ret != 0)
    {
        printf("Set and verfiy core boostlimit failed!\n");
        return ret;
    }
    printf("Set and Verify Core boostlimit to %uMHz: Success\n", boostlimit);

    return 0;
}
