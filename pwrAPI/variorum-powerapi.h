#ifndef _VARIORUM_PWRAPI_H
#define _VARIORUM_PWRAPI_H

#include <stdio.h>
#include <stdlib.h>
#include "pwrtypes.h"
#include <jansson.h>
#include "../src/variorum/variorum.h"
#include "../src/variorum/variorum_topology.h"
#include <time.h>
#include <string.h>

//extract min or max watt values from control range 
double parse(const char* minmax, const char* input) {
    int i = 0;
    int first, last;
    if(strcmp(minmax, "min") == 0) {
        while(input[i] != '\0') {
            if(input[i] == ',') {
                last = i-1;
                break;
            } else if (input[i] == 'i') {
                first = i+4;
            }
            i++;
        }
    } else {
        while(input[i] != '\0') {
            if(input[i] == '}') {
                last = i-1;
                break;
            } else if (input[i] == 'a') {
                first = i+4;
            }
            i++;
        }
    }
    int len = last-first+2;
    char value[len];
    memset(value, '\0', len);
    
    char *valuestr;
    for(int i = 0, j = first; i < len && j <= last; ++i, ++j) {
        value[i] = input[j];
    }

    return strtod(value, &valuestr);
}

typedef struct {
	PWR_ObjType type;
	int misc;
	char option[4];
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

	char* domain = NULL;
	int domain_ret = variorum_get_node_power_domain_info_json(&domain);
	json_t *domain_obj = json_loads(domain, JSON_DECODE_ANY, NULL);
	json_t*  ranges = json_object_get(domain_obj, "control_range");
	const char* range = json_string_value(ranges);
	
	double min_power = parse("min", range);
	double max_power = parse("max", range);


	//printf("minimum power is %lf, maximum power is %lf\n", min_power, max_power);
		
    	if (num_sockets <= 0)
    	{
            printf("HWLOC returned an invalid number of sockets. Exiting.\n");
            exit(-1);
    	}
	

	if (ret != 0)
	{
		fprintf(stdout, "Unable to obtain power details.");
		exit(-1);
	}

	if (domain_ret != 0) 

	{
		fprintf(stdout, "Unable to obtain domain details.");
		exit(-1);
	}


	switch(obj_type) {
		case PWR_OBJ_NODE:
			if(type == PWR_ATTR_POWER) {
				*( (double *)ptr ) = json_real_value(json_object_get(power_obj, "power_node_watts")); 
			}

			else if (type == PWR_ATTR_POWER_LIMIT_MIN) {
				*( (double *)ptr ) = min_power;
			}

			else if (type == PWR_ATTR_POWER_LIMIT_MAX) {
				*( (double *)ptr ) = max_power;
			}
			else {
				printf("Attribute not supported!\n");
			}
			break;

		case PWR_OBJ_MEM:
		    	if(type == PWR_ATTR_POWER) {
				double mem_power = 0;
				char mem_name[25];
				strcpy(mem_name, "power_mem_watts_socket_");
				for(int i = 0; i < num_sockets; ++i){
					mem_name[23] = i + '0';
					mem_name[24] = '\0';
					//printf("%s\n", mem_name);
					mem_power += json_real_value(json_object_get(power_obj, mem_name));
				}

				*( (double*)ptr ) = mem_power;			
		    	}
			else {
				printf("Attribute not supported!\n");
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
					socket_cpu[24] = '\0';
					socket_gpu[24] = '\0';
					socket_mem[24] = '\0';
					double _cpu_power = json_real_value(json_object_get(power_obj, socket_cpu) );
					double _gpu_power = json_real_value(json_object_get(power_obj, socket_gpu) );
					double _mem_power = json_real_value(json_object_get(power_obj, socket_mem) );

					socket_power += _cpu_power < 0? 0 : _cpu_power;
					socket_power += _gpu_power < 0? 0 : _gpu_power;
					socket_power += _mem_power < 0? 0 : _mem_power;
					//printf("socket power is %lf\n", socket_power);
					*( (double*)ptr ) = socket_power;

				}
			}
			else {
				printf("Attribute not supported!\n");
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
