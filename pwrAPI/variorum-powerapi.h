#ifndef _VARIORUM_PWRAPI_H
#define _VARIORUM_PWRAPI_H

#include <stdio.h>
#include <stdlib.h>
#include "pwrtypes.h"
#include <jansson.h>
#include "../../src/variorum/variorum.h"
#include "../../src/variorum/variorum_topology.h"
#include <time.h>
#include <string.h>

typedef struct {
	PWR_ObjType type;
	int misc;
} Object;

typedef struct {
	Object* root;
} Cntxt;


int PWR_CntxtInit(PWR_Cntxt type, PWR_Role role, const char* name, PWR_Cntxt* ctx) {

	if(role != PWR_ROLE_INVALID && role != PWR_ROLE_NOT_SPECIFIED) {
		*ctx = malloc(sizeof(Cntxt));
		if(*ctx == NULL) {
			return PWR_RET_FAILURE;
		}
	} else {
		return PWR_RET_FAILURE;
	}
	return PWR_RET_SUCCESS;	
}

int PWR_CntxtDestory(PWR_Cntxt ctx) {
	free(ctx);
	return PWR_RET_SUCCESS;
}

int PWR_CntxtGetEntryPoint(PWR_Cntxt ctx, PWR_Obj* obj) {
	*obj = ((Cntxt*)ctx)->root;	
	return PWR_RET_SUCCESS;
}

int PWR_ObjAttrGetValue(PWR_Obj obj, PWR_AttrName type, void* ptr, PWR_Time* ts )

{

	//get object type and misc value
	//currently using misc value to represent socket #
	PWR_ObjType obj_type = ((Object *)obj)->type;
	int misc = ((Object *)obj)->misc;
	
	char* s = NULL;
	int num_sockets;
	int ret = variorum_get_node_power_json(&s);
	json_t *power_obj = json_loads(s, JSON_DECODE_ANY, NULL);

	num_sockets = variorum_get_num_sockets();

    if (num_sockets <= 0)
    {
        printf("HWLOC returned an invalid number of sockets. Exiting.\n");
        exit(-1);
    }
	
	ret = variorum_get_node_power_json(&s);

	if (ret != 0)
	{
		fprintf(stdout, "Unable to obtain power details.");
		exit(-1);
	}


	switch(obj_type) {
		case PWR_OBJ_NODE:
			if(type == PWR_ATTR_POWER) {
				uint64_t watts = json_real_value(json_object_get(power_obj, "power_node_watts") );
				printf("node watts: %ld\n", watts);
				*( (double *)ptr ) = json_real_value(json_object_get(power_obj, "power_node_watts")); 
			}
			break;

		case PWR_OBJ_MEM:
		    if(type == PWR_ATTR_POWER) {
				double mem_power = 0;
				char mem_name[25];
				strcpy(mem_name, "power_mem_watts_socket_");
				for(int i = 0; i < num_sockets; ++i){
					mem_name[23] = i + '0';
					mem_power += json_real_value(json_object_get(power_obj, mem_name));
				}

				*( (double*)ptr ) = mem_power;			
			}
			break;

		case PWR_OBJ_SOCKET:
			if(type == PWR_ATTR_POWER) {
				if(misc >= num_sockets) {
					return PWR_RET_FAILURE;
				} else {
					double socket_power = 0;
					char socket_cpu[25];
					char socket_gpu[25];
					char socket_mem[25];
					strcpy(socket_cpu, "power_cpu_watts_socket_");
					strcpy(socket_gpu, "power_gpu_watts_socket_");
					strcpy(socket_mem, "power_mem_watts_socket_");
					socket_cpu[23] = misc + '0';
					socket_gpu[23] = misc + '0';
					socket_mem[23] = misc + '0';
					socket_power += json_real_value(json_object_get(power_obj, socket_cpu) );
					socket_power += json_real_value(json_object_get(power_obj, socket_gpu) );
					socket_power += json_real_value(json_object_get(power_obj, socket_mem) );
					printf("socket power is %lf\n", socket_power);
					*( (double*)ptr ) = socket_power;

				}
			}
			break;

		default:
			printf("type not supported!\n");
			break;
	}

	//set time stamp for PWR_Time
	*ts = json_integer_value(json_object_get(power_obj, "timestamp"));
	
	json_decref(power_obj);
	free(s);


	return PWR_RET_SUCCESS;			
}

int PWR_ObjAttrSetValue( PWR_Obj obj, PWR_AttrName type, const void* ptr )
{
    PWR_ObjType obj_type = ((Object *)obj)->type;
	const int* watt_limit = (const int*)ptr;
    int ret;

    switch(obj_type) {
	    case PWR_OBJ_NODE:
		    if(type == PWR_ATTR_POWER) {
				ret = variorum_cap_best_effort_node_power_limit(*watt_limit);
			}
			if(ret) {
			    printf("power limit setting failed!\n");
				return PWR_RET_FAILURE;
		    }
			break;
		
		default:
		    printf("type not supported!\n");
			break;
	}

	return PWR_RET_SUCCESS;
}




#endif 
