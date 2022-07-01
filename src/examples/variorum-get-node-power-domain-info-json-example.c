// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <variorum.h>

int main(void)
{
    int ret;
    char *s = NULL;

    /* Allocate string based on vendor-neutral JSON structure*/
    s = (char *) malloc(800 * sizeof(char));

    ret = variorum_get_node_power_domain_info_json(&s);

    if (ret != 0)
    {
        printf("First run: JSON get node power domain information failed!\n");
    }

    /* Print the entire JSON object */
    puts(s);

    /* Deallocate the string */
    free(s);

    return ret;
}
