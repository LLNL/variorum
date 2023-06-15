#ifndef _PWR_TYPES_H
#define _PWR_TYPES_H

#include <stdint.h>

#define PWR_RET_SUCCESS 0
#define PWR_RET_FAILURE -1
typedef void* PWR_Cntxt;
typedef void* PWR_Grp;
typedef void* PWR_Obj;
typedef void* PWR_Status;
typedef void* PWR_Stat;
typedef uint64_t PWR_Time;


//implementation supports only NODE, SOCKET, CORE, MEM
typedef enum {
    PWR_OBJ_PLATFORM = 0,
    PWR_OBJ_CABINET, 
    PWR_OBJ_CHASSIS, 
    PWR_OBJ_BOARD,
    PWR_OBJ_NODE,
    PWR_OBJ_SOCKET,
    PWR_OBJ_CORE,
    PWR_OBJ_POWER_PLANE,
    PWR_OBJ_MEM,
    PWR_OBJ_NIC,

    PWR_OBJ_TX2_CORE,
    PWR_OBJ_TX2_SRAM,
    PWR_OBJ_TX2_MEM,
    PWR_OBJ_TX2_SOC,

    PWR_NUM_OBJ_TYPES,
    /* */
    PWR_OBJ_INVALID = -1, 
    PWR_OBJ_NOT_SPECIFIED = -2 
} PWR_ObjType;

typedef enum {
	PWR_ATTR_PSTATE = 0, /* uint64_t */
	PWR_ATTR_CSTATE, /* uint64_t */
	PWR_ATTR_CSTATE_LIMIT, /* uint64_t */
	PWR_ATTR_SSTATE, /* uint64_t */
	PWR_ATTR_CURRENT, /* double, amps */
	PWR_ATTR_VOLTAGE, /* double, volts */
	PWR_ATTR_POWER, /* double, watts */
	PWR_ATTR_POWER_LIMIT_MIN, /* double, watts */
	PWR_ATTR_POWER_LIMIT_MAX, /* double, watts */
	PWR_ATTR_FREQ, /* double, Hz */
	PWR_ATTR_FREQ_LIMIT_MIN, /* double, Hz */
	PWR_ATTR_FREQ_LIMIT_MAX, /* double, Hz */
	PWR_ATTR_ENERGY, /* double, joules */
	PWR_ATTR_TEMP, /* double, degrees Celsius */
	PWR_ATTR_OS_ID, /* uint64_t */
	PWR_ATTR_THROTTLED_TIME, /* uint64_t */
	PWR_ATTR_THROTTLED_COUNT, /* uint64_t */
	PWR_ATTR_GOV, /* uint64_t */
	PWR_NUM_ATTR_NAMES,/* */
	PWR_ATTR_INVALID = -1,
	PWR_ATTR_NOT_SPECIFIED = -2
} PWR_AttrName;

typedef enum {
    PWR_ROLE_APP = 0, /* Application */
    PWR_ROLE_MC,      /* Monitor and Control */
    PWR_ROLE_OS,      /* Operating System */
    PWR_ROLE_USER,    /* User */
    PWR_ROLE_RM,      /* Resource Manager */
    PWR_ROLE_ADMIN,   /* Administrator */
    PWR_ROLE_MGR,     /* HPCS Manager */
    PWR_ROLE_ACC,     /* Accounting */
    PWR_NUM_ROLES,    /* Accounting */
    /* */
    PWR_ROLE_INVALID = -1, 
    PWR_ROLE_NOT_SPECIFIED = -2 
} PWR_Role;


#endif
