#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = print_features();
    if (ret != 0)
    {
        printf("Print features failed!\n");
    }
    return ret;
}
