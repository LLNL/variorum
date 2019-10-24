#include <stdio.h>
#include <stdlib.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;
    int node_pow_lim;

    if (argc == 1)
    {
        printf("No power limit specified...using default limit of 500W.\n");
        node_pow_lim = 500; /*500 W is based on minimum power on IBM Witherspoon */
    }
    else if (argc == 2)
    {
        node_pow_lim = atoi(argv[1]);
        printf("Setting node to %dW.\n", node_pow_lim);
    }

    ret = set_node_power_limit(node_pow_lim);
    if (ret != 0)
    {
        printf("Set node power limit failed!\n");
    }
    printf("\n");
    ret = dump_power_limits();
    if (ret != 0)
    {
        printf("Dump power limits failed!\n");
    }
    return ret;
}
