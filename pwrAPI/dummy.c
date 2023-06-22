#include <stdio.h>
#include "variorum-powerapi.h"


int main() {

	Object obj;
	obj.type = PWR_OBJ_NODE;
	PWR_Time now;
	double value = 0;
	PWR_AttrName type = PWR_ATTR_POWER;
	int ret = PWR_ObjAttrGetValue(&obj, type, &value, &now);
	printf("The node power is %lf Watts and the time is %ld\n", value, now);

    	Object mem;
	mem.type = PWR_OBJ_MEM;
	PWR_Time now2;
	double value2 = 0;
	ret = PWR_ObjAttrGetValue(&mem, type, &value2, &now2);
	printf("Memory power is %lf Watts and the time is %ld\n", value2, now2);

	Object limit;
	limit.type = PWR_OBJ_NODE;
	int limit_value = 2500;
	ret = PWR_ObjAttrSetValue(&limit, type, &limit_value);
	if(ret == PWR_RET_SUCCESS) {
		printf("Power Limit Set Success!\n");
	} else {printf("Power Limit Set Failed!\n");}

	Object socket0;
	socket0.type = PWR_OBJ_SOCKET;
	socket0.misc = 0;
	PWR_Time now3;
	double value3 = 0;
	ret = PWR_ObjAttrGetValue(&socket0, type, &value3, &now3);
	if(ret == PWR_RET_SUCCESS) {
		printf("Socket 0 power is  %lf watts and the time is %ld\n", value3, now3);
	}

	Object socket1;
    	socket1.type = PWR_OBJ_SOCKET;
    	socket1.misc = 1;
	PWR_Time now4;
	double value4 = 0;
    	ret = PWR_ObjAttrGetValue(&socket1, type, &value4, &now4 );
	if(ret == PWR_RET_SUCCESS) {
		  printf("Socket 1 power is  %lf watts and the time is %ld\n", value4, now4);
    	}


	Object node_min_power;
	node_min_power.type = PWR_OBJ_NODE;
	strcpy(node_min_power.option, "min");
	PWR_Time now5;
	double value5 = 0;
	type = PWR_ATTR_POWER_LIMIT_MIN;
	ret = PWR_ObjAttrGetValue(&node_min_power, type, &value5, &now5);
	if(ret == PWR_RET_SUCCESS) {
		printf("Node minimum power is %lf watts and the time is %ld\n", value5, now5);
	}	

	Object node_max_power;
	node_max_power.type = PWR_OBJ_NODE;
	strcpy(node_min_power.option, "max");
	PWR_Time now6;
	double value6 = 0;
	type = PWR_ATTR_POWER_LIMIT_MAX;
	ret = PWR_ObjAttrGetValue(&node_max_power, type, &value6, &now6);
	if(ret == PWR_RET_SUCCESS) {
		printf("Node maximum power is %lf watts and the time is %ld\n", value6, now6);
	}

	return 0;

}
