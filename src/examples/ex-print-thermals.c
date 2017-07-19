#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = print_thermals();
    if (ret != 0)
    {
        printf("Print thermals failed!\n");
    }
    return ret;
}
