#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    ret = print_package_frequency();
    if (ret != 0)
    {
        printf("Print package_frequency failed!\n");
    }
#ifdef SECOND_RUN
    ret = print_package_frequency();
#endif
    return ret;
}
