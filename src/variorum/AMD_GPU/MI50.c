// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <MI50.h>
#include <config_architecture.h>
#include <variorum_error.h>
#include <power_features.h>

int mi50_get_power(int long_ver)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    int iter = 0;
    int nsockets;

    variorum_get_topology(&nsockets, NULL, NULL);

    for (iter = 0; iter < nsockets; iter++)
    {
        get_power_data(iter, nsockets, long_ver, stdout);
    }
    return 0;
}
