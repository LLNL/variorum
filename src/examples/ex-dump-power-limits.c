#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = dump_power_limits();
    if (ret != 0)
    {
        printf("Dump power limits failed!\n");
    }
    return ret;
}
