#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = poll_power();
    if (ret != 0)
    {
        printf("Poll power failed!\n");
    }
    return ret;
}
