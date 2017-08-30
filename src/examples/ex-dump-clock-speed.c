#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = dump_clock_speed();
    if (ret != 0)
    {
        printf("Dump clock speed failed!\n");
    }
    return ret;
}
