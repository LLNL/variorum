// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <variorum.h>
#include <variorum_topology.h>

int main(void)
{
    int ret;
    char *s = NULL;

    /* Allocate string based on number of sockets on the platform */
    /* String allocation below assumes the following: 
     * Upper bound of 180 characters for hostname, timestamp and node power.
     * Upper bound of 150 characters for per-socket information */
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
