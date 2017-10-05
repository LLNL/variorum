#include <stdio.h>
#include <variorum.h>

int main(int argc, char **argv)
{
    int ret;

	ret = disable_turbo();
	if (ret != 0)
    {
		printf("Disable turbo failed!\n");
	}
	ret = enable_turbo();
	if (ret != 0)
    {
		printf("Enable turbo failed!\n");
	}
    return ret;
}
