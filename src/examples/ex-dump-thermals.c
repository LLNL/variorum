#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = dump_thermals();
    if (ret != 0)
    {
        printf("Dump thermals failed!\n");
    }
    return ret;
}
