#ifndef POWER9_H_INCLUDE
#define POWER9_H_INCLUDE

int p9_get_power(void);

int p9_get_power_limits(void);

int p9_set_power_limit(int pcap_new);

int p9_set_and_verify_power_limit(int pcap_new);

int p9_monitoring(FILE *output);

#endif
