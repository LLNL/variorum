// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include <variorum_config.h>
#include <getopt.h>
#include <variorum.h>

int main(int argc, char **argv)
{
    const char *usage = "Usage: %s [-hv] \n";
    int opt;
    while ((opt = getopt(argc, argv, "hv")) != -1)
    {
        switch (opt)
        {
            case 'v':
                printf("%s\n", variorum_get_current_version());
                return 0;
                break;
            case 'h':
                printf(usage, argv[0]);
                return 0;
                break;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }
    int ret;
    char *s = NULL;

    /* Allocate string based on vendor-neutral JSON structure*/
    s = (char *) malloc(800 * sizeof(char));

    ret = variorum_get_node_power_domain_info_json(&s);

    if (ret != 0)
    {
        printf("JSON get node power domain information failed!\n");
        free(s);
        exit(-1);
    }

    /* Print the entire JSON object */
    puts(s);

    /* Deallocate the string */
    free(s);

    return ret;
}
