#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = print_counters();
    if (ret != 0)
    {
        printf("Print counters failed!\n");
    }
    return ret;
}
