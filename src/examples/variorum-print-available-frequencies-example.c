#include <stdio.h>

#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

    /* 02/25/19 SB
     * How do we distinguish errors if this function pointer does not exist
     * because it has not yet been implemented or if it cannot be implemented?
     */
    ret = variorum_print_available_frequencies();
    if (ret != 0)
    {
        printf("Print available frequencies failed!\n");
    }
    return ret;
}
