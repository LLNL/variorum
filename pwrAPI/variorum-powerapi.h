#ifndef _VARIORUM_PWRAPI_H
#define _VARIORUM_PWRAPI_H

#include <stdio.h>
#include <stdlib.h>
#include "pwrtypes.h"
#include <jansson.h>
#include "../../src/variorum/variorum.h"
#include "../../src/variorum/variorum_topology.h"
#include <time.h>

typedef struct {
	PWR_ObjType type;
	int misc;
} Object;

typedef struct {
	Object* root;
} Cntxt;

void parse_json_obj(char *s, int num_sockets, double *node_pwr, double *cpu_pwr, double *gpu_pwr, double *mem_pwr)
{
    int i, j;

    /* Create a Jansson based JSON structure */
    json_t *power_obj = NULL;
    static char **json_metric_names = NULL;

    /* List of socket-level JSON object metrics */
    static const char *metrics[] = {"power_cpu_watts_socket_",
                                    "power_gpu_watts_socket_",
                                    "power_mem_watts_socket_"
                                   };

    /* Allocate space for metric names */
    json_metric_names = malloc(3 * num_sockets * sizeof(char *));
    for (i = 0; i < (3 * num_sockets); i++)
    {
        json_metric_names[i] = malloc(40);
    }

    for (i = 0; i < num_sockets; i++)
    {
        /* Create a metric name list for querying json object later on */
        for (j = 0; j < 3; j++)
        {
            char current_metric[40];
            char current_socket[16];
            strcpy(current_metric, metrics[j]);
            sprintf(current_socket, "%d", i);
            strcat(current_metric, current_socket);
            strcpy(json_metric_names[(num_sockets * j) + i], current_metric);
        }
    }

    /* Load the string as a JSON object using Jansson */
    power_obj = json_loads(s, JSON_DECODE_ANY, NULL);

    /* Extract and print values from JSON object */
    *node_pwr = json_real_value(json_object_get(power_obj, "power_node_watts"));

    for (i = 0; i < num_sockets; i++)
    {
        *cpu_pwr += json_real_value(json_object_get(power_obj, json_metric_names[i]));
        *gpu_pwr += json_real_value(json_object_get(power_obj,
                                    json_metric_names[num_sockets + i]));
		*mem_pwr += json_real_value(json_object_get(power_obj,
                                    json_metric_names[(num_sockets * 2) + i]));
    }

    /* Deallocate metric array */
    for (i = 0; i < num_sockets * 3; i++)
    {
        free(json_metric_names[i]);
    }
    free(json_metric_names);

    /*Deallocate JSON object*/
    json_decref(power_obj);
}

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
}

int PWR_ObjAttrGetValue(PWR_Obj obj, PWR_AttrName type, void* ptr, PWR_Time* ts )

{

	//get object type and misc value
	//currently using misc value to represent socket #
	PWR_ObjType obj_type = ((Object *)obj)->type;
	int misc = ((Object *)obj)->misc;

	double node_power = 0.0; 
	double gpu_power = 0.0;
	double cpu_power = 0.0;
	double mem_power = 0.0;
	time_t variorum_ts;
	char* s = NULL;
	int num_sockets;
	int ret;
	
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

	parse_json_obj(s, num_sockets, &node_power, &cpu_power, &gpu_power, &mem_power);

	free(s);

	switch(obj_type) {
		case PWR_OBJ_NODE:
			if(type == PWR_ATTR_POWER) {
				*( (uint64_t *)ptr ) = node_power; 
			}
			break;

		default:
			printf("type not supported!\n");
			break;
	}

	//set time stamp for PWR_Time
	*ts = time(NULL);


	return 0;			
}

int PWR_ObjAttrSetValue( PWR_Obj obj, PWR_AttrName type, const void* ptr )
{
    //return OBJECT(obj)->attrSetValue( type, (void*)ptr );
	return 0;
}




#endif 

