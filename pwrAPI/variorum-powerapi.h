#ifndef _VARIORUM_PWRAPI_H
#define _VARIORUM_PWRAPI_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pwrtypes.h"
#include <jansson.h>
#include "../src/variorum/variorum.h"
#include "../src/variorum/variorum_topology.h"
#include <time.h>
#include <string.h>


//General object struct
typedef struct {
	PWR_ObjType type;
	int misc; // use misc for object type specific info such as socket number or core number or dram slot number
	char option[4]; //use option to differentiate between different usecases of the same time. eg Object Type = Core, option = "CPU" or "GPU";
	double value;
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
	char *option = ((Object *)obj)->option;
	char* node_power  = NULL;
	char* node_domain  = NULL;
	int power_ret = variorum_get_node_power_json(&node_power);
	int domain_ret = variorum_get_node_power_domain_info_json(&node_domain);
	char hostname[1024];

	gethostname(hostname, 1024);

	if (power_ret != 0)
	{
		fprintf(stdout, "Unable to obtain power details!\n");
		exit(-1);
	}
	if (domain_ret != 0)
	{
		fprintf(stdout, "Unable to obtain domain details!\n");
		exit(-1);
	}
	json_t *power_obj = json_loads(node_power, JSON_DECODE_ANY, NULL);
	json_t *domain_obj = json_loads(node_domain, JSON_DECODE_ANY, NULL);

	switch(obj_type) 	
	{
		case PWR_OBJ_NODE:
			if (type == PWR_ATTR_POWER)
			{
				json_t *node_obj = json_object_get(power_obj, hostname);
				*( (double *)ptr ) = json_real_value(json_object_get(node_obj, "power_node_watts") );
				*ts = json_integer_value(json_object_get(node_obj, "timestamp"));
			} else if (type == PWR_ATTR_POWER_LIMIT_MIN)
			{
				json_t *node_obj = json_object_get(domain_obj, hostname);
				json_t *control_obj = json_object_get(node_obj, "control");
				json_t *node_power_obj = json_object_get(control_obj, "power_node");
				*( (double *)ptr ) = json_number_value(json_object_get(node_power_obj, "min") );
				*ts = json_integer_value(json_object_get(node_obj, "timestamp"));
			} else if (type == PWR_ATTR_POWER_LIMIT_MAX)
			{	
				json_t *node_obj = json_object_get(domain_obj, hostname);
				json_t *control_obj = json_object_get(node_obj, "control");
				json_t *node_power_obj = json_object_get(control_obj, "power_node");
				*( (double *)ptr ) = json_number_value(json_object_get(node_power_obj, "max") );
				*ts = json_integer_value(json_object_get(node_obj, "timestamp"));
			} else 
			{
				printf("Attribute type %d not supported!\n", type);
				return PWR_RET_FAILURE;
			}
			break;

		case PWR_OBJ_MEM:
			if (type == PWR_ATTR_POWER)
			{ 
				json_t *node_obj = json_object_get(power_obj, hostname);
				void *iter = json_object_iter(node_obj);
				double mem_power_val = 0.0;
				while(iter != NULL){
					if(strncmp(json_object_iter_key(iter), "Socket", 6) == 0)
					{
						json_t *socket_obj = json_object_iter_value(iter);
						double mem_power_temp = json_number_value(json_object_get(socket_obj, "power_mem_watts"));
						mem_power_val += mem_power_temp > 0.0 ? mem_power_temp : 0.0;
						}
					iter = json_object_iter_next(node_obj, iter);
				}
				*( (double *)ptr ) = mem_power_val;
				*ts = json_integer_value(json_object_get(node_obj, "timestamp"));
			} else 
			{
				printf("Attribute type %d not supported!\n", type);
				return PWR_RET_FAILURE;
			}
			break;

		case PWR_OBJ_SOCKET:
			if (type == PWR_ATTR_POWER)
			{
				json_t *node_obj = json_object_get(power_obj, hostname);
				char socketID[16];
				snprintf(socketID, 16, "Socket_%d", misc);
				json_t *socket_obj = json_object_get(node_obj, socketID);
				if (socket_obj == NULL)
				{
					printf("Error: %s not found in json object!\n", socketID);
					return PWR_RET_FAILURE;
				}
				double socket_pwr_value = 0.0;
				void *iter = json_object_iter(socket_obj);
				while(iter != NULL)
				{
					double socket_power_temp = json_number_value(json_object_iter_value(iter));
					socket_pwr_value += socket_power_temp > 0.0 ? socket_power_temp : 0.0;
					iter = json_object_iter_next(socket_obj, iter);
				}
				*( (double *)ptr ) = socket_pwr_value;
				*ts = json_integer_value(json_object_get(node_obj, "timestamp"));
			} else 
			{
				printf("Attribute type %d not supported!\n", type);
				return PWR_RET_FAILURE;
			}

			break;
	
		case PWR_OBJ_CORE:
			if (type == PWR_ATTR_POWER)
			{
				json_t *node_obj = json_object_get(power_obj, hostname);
				char socketID[16];
				snprintf(socketID, 16, "Socket_%d", misc);
				json_t *socket_obj = json_object_get(node_obj, socketID);
				if (socket_obj == NULL)
				{
					printf("Error: %s not found in json object!\n", socketID);
					return PWR_RET_FAILURE;
				}
				if (strcmp(option, "CPU") == 0)
				{
					*( (double *)ptr ) = json_real_value(json_object_get(socket_obj, "power_cpu_watts"));
				} 
				else if (strcmp(option, "GPU") == 0)
				{
					*( (double *)ptr ) = json_real_value(json_object_get(socket_obj, "power_gpu_watts"));
				}
				*ts = json_integer_value(json_object_get(node_obj, "timestamp"));
			} else 
			{
				printf("Attribute type %d not supported!\n", type);
				return PWR_RET_FAILURE;
			}

			break;

		default:
			printf("object type %d not supported!\n", type);
			break;	
	}
	json_decref(power_obj);
	json_decref(domain_obj);
	free(node_power);
	free(node_domain);

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
