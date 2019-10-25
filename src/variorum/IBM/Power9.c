#include <stdio.h>
#include <stdlib.h>

#include <config_architecture.h>
#include <Power9.h>
#include <variorum_error.h>

// #define FILENAME_SIZE 1024

int p9_get_power(void)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
}

int p9_get_power_limits(void)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif
    // fprintf(stdout, "Entered %s\n", __FUNCTION__);
    char hostname[1024];
    FILE *fp = NULL;
    int pcap_current = 0;
    int pcap_max = 0;
    int pcap_min = 0;
    int psr_1 = 0;
    int psr_2 = 0;

    gethostname(hostname, 1024);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current", "r");
    fscanf(fp, "%d", &pcap_current);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-max", "r");
    fscanf(fp, "%d", &pcap_max);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-min", "r");
    fscanf(fp, "%d", &pcap_min);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/psr/cpu_to_gpu_0", "r");
    fscanf(fp, "%d", &psr_1);
    fclose(fp);

    fp = fopen("/sys/firmware/opal/psr/cpu_to_gpu_0", "r");
    fscanf(fp, "%d", &psr_2);
    fclose(fp);

    fprintf(stdout, "_POWERCAP | CurrentWatts | MaxWatts | MinWatts | CPU_to_GPU_0 PSR | CPU_to_GPU_8 PSR\n");
    fprintf(stdout, "_POWERCAP | %d | %d | %d | %d | %d \n", pcap_current, pcap_max, pcap_min, psr_1, psr_2);
    return 0;
}

int p9_set_and_verify_power_limit(int pcap_new)
{
#ifdef VARIORUM_LOG
    printf("Running %s with value %d\n", __FUNCTION__, pcap_new);
#endif

    char hostname[1024];
    FILE *fp = NULL;
    int pcap_test = 0;

    gethostname(hostname, 1024);

    /* Strange problem and portability note from T. Patki, 11/27/18, Alehouse:
     * Here, we are doing a write followed by a read. If we want to do this,
     * usleep is needed because file operations seem to take some time to update and
     * return (most likely cause is a lazy-write and seek time). After a bit of testing, 10ms seems like
     * a good sleep time for alehouse to finish doing an fprintf and allow for a fscanf.
     * Using the same file pointer doesn't work, file has to be closed and reopened. I'm afraid
     * the 10ms value may not be portable. I've tried reusing the file pointer
     * with rewind/fseek/fflush, and that didn't help. Maybe I'm missing something.
     * We could completely bypass this approach of write-and-test, and only go with a simple fprintf
     * but that would not be the most user-friendly and we will not be able to confirm a power cap change.
     * Running this function back-to-back can also cause issues.
     * We can decide later what's best for production.
     * */

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current", "w+");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files", VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    fprintf(fp, "%d", pcap_new);
    fclose(fp);

    usleep(10000);

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current", "r");
    fscanf(fp, "%d", &pcap_test);
    fclose(fp);
    // fprintf(stdout, "Values are input=%d, test=%d\n", pcap_new, pcap_test);

    if (pcap_new != pcap_test)
    {
        fprintf(stdout, "Could not set power cap correctly.\n");
        return 1;
    }

    fprintf(stdout, "Changed node power cap to %d W.\n", pcap_new);
    return 0;
}

int p9_set_power_limit(int pcap_new)
{
#ifdef VARIORUM_LOG
    printf("Running %s with value %d\n", __FUNCTION__, pcap_new);
#endif

    char hostname[1024];
    FILE *fp = NULL;
    int pcap_test = 0;

    gethostname(hostname, 1024);

    /*  Strange problem and portability note from T. Patki, 11/27/18, Alehouse:
     * Here, we are doing a write and not verifying whether the power cap was set.
     * Introduced as per Barry's suggestion, so users can decide if they want a 10ms delay
     * that we need for verification.
     * */

    fp = fopen("/sys/firmware/opal/powercap/system-powercap/powercap-current", "w+");
    if (fp == NULL)
    {
        variorum_error_handler("Incorrect permissions on OPAL files", VARIORUM_ERROR_INVAL, getenv("HOSTNAME"), __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    fprintf(fp, "%d", pcap_new);
    fclose(fp);

    fprintf(stdout, "Changed node power cap to %d W.\n", pcap_new);
    return 0;
}

int p9_monitoring(FILE *output)
{
#ifdef VARIORUM_LOG
    printf("Running %s\n", __FUNCTION__);
#endif

    // @todo There will be repeat implementation from the get_power API above.
    // We might want to extract that out into a static P9-get-power function,
    // and call then from the p9_monitoring() and p9_get_power() APIs.
    fprintf(output, "to be implemented\n");

    return 0;
}
