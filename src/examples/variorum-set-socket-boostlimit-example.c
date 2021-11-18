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
    int socket;
    unsigned int boostlimit = 0;

    printf("\nUsage: %s <socket> <boostlimit>\n", argv[0]);

    if (argc == 1)
    {
        printf("socket number not specified...using default socket 0\n");
        socket = 0;
        printf("No boostlimit specified...using default limit of 400MHz.\n");
        boostlimit = 400;
    }
    else if (argc == 2)
    {
        socket = atoi(argv[1]);
        printf("Specified socket num : %d\n", socket);
        printf("No boostlimit specified...using default limit of 400MHz.\n");
        boostlimit = 400;
    }
    else if (argc > 2)
    {
        socket = atoi(argv[1]);
        boostlimit = atoi(argv[2]);
        printf("Setting boostlimit for socket[%d] to %uMHz\n",
               socket, boostlimit);
    }

    ret = variorum_set_socket_boostlimit(socket, boostlimit);
    if (ret != 0)
    {
        printf("Set boostlimit failed!\n");
        return ret;
    }

    return 0;
}
