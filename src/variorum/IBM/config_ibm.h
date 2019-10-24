#ifndef CONFIG_IBM_H_INCLUDE
#define CONFIG_IBM_H_INCLUDE

#include <inttypes.h>

uint64_t *detect_ibm_arch(void);

int set_ibm_func_ptrs(void);

#endif
