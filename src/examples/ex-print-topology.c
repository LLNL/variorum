#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = print_topology();
    if (ret != 0)
    {
        printf("Print topology failed!\n");
    }
    return ret;
}
