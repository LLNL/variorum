#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = print_power_limits();
    if (ret != 0)
    {
        printf("Print power limits failed!\n");
    }
    return ret;
}
