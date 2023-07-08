// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <stdio.h>

#include <variorum.h>

#ifdef SECOND_RUN
static inline double do_work(int input)
{
    int i;
    double result = (double)input;

    for (i = 0; i < 100000; i++)
    {
        result += i + result;
    }

    return result;
}
#endif

enum power_domain {NODE = 0, PROCESSOR, MEMORY, GPU};

int main(int argc, char **argv)
{
    double power_value;
    enum power_domain domain = NODE;

#ifdef SECOND_RUN
    int i;
    int size = 1E3;
    volatile double x = 0.0;
#endif

    const char *usage = "Usage: %s [-h] [-v]\n";
    int opt;
    while ((opt = getopt(argc, argv, "hv")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf(usage, argv[0]);
                return 0;
            case 'v':
                printf("%s\n", variorum_get_current_version());
                return 0;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }

    // Get power data from Node domain, deviceID is irrelevant and ignored here.
    power_value = variorum_get_domain_power_value(NODE, 0);
    printf("Power for the Node domain is %lf watts\n", power_value);

    // Get power data from Socket domain and specify device (socket) ID as 1.
    power_value = variorum_get_domain_power_value(PROCESSOR, 1);
    printf("Processor Power for Socket 1 is %lf watts\n", power_value);

    // Get power data from Memory domain and specify device (memory DIMM) ID as 0.
    power_value = variorum_get_domain_power_value(MEMORY, 0);
    printf("Power for Memory from Socket 0 is  %lf watts\n", power_value);

/*
    // Get power data from GPU domain and specify device (GPU) ID as 2.
    power_value = variorum_get_domain_power_value(GPU, 2);
    printf("Power for the GPU ID 2 is %lf watts\n", power_value);

    if (power_value == -1.0)
    {
        printf("Variorum get_domain_power_value failed!\n");
    }

#ifdef SECOND_RUN
    for (i = 0; i < size; i++)
    {
        x += do_work(i);
    }
    printf("Final result: %f\n", x);

    // Get power data from Node domain, deviceID is irrelevant and ignored here.
    power_value = variorum_get_domain_power_value(NODE, 0);
    printf("Power for the Node domain is %lf watts\n", power_value);

    // Get power data from Socket domain and specify device (socket) ID as 1.
    power_value = variorum_get_domain_power_value(SOCKET, 1);
    printf("Power for Socket 1 is %lf watts\n", power_value);

    // Get power data from Memory domain and specify device (memory DIMM) ID as 0.
    power_value = variorum_get_domain_power_value(MEMORY, 0);
    printf("Power for the Memory DIMM 0 is  %lf watts\n", power_value);

    // Get power data from GPU domain and specify device (GPU) ID as 2.
    power_value = variorum_get_domain_power_value(GPU, 2);
    printf("Power for the GPU ID 2 is %lf watts\n", power_value);

    if (power_value == -1.0)
    {
        printf("Variorum get_domain_power_value failed!\n");
    }
#endif
*/

    return 0;
}
