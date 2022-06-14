// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <variorum_config.h>
#include <getopt.h>
#include <variorum.h>

int main(int argc, char **argv)
{
    variorum_print_topology();

    const char *usage = "Usage: %s [-h] \n";
    int opt;
    while ((opt = getopt(argc, argv, "hvt")) != -1)
    {
        switch (opt)
        {
            case 'v':
                printf(QuoteMacro(VARIORUM_VERSION)"\n");
                return 0;
                break;
            case 'h':
                printf(usage, argv[0]);
                return 0;
                break;
	    case 't':
    		variorum_print_topology();
		return 0;
		break;
            default:
                fprintf(stderr, usage, argv[0]);
                return -1;
        }
    }
    return 0;
}
