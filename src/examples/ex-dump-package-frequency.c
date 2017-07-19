#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = dump_package_frequency();
    if (ret != 0)
    {
        printf("Dump package frequency failed!\n");
    }
    return ret;
}
