#include <stdio.h>
#include "variorum-powerapi.h"


int main() {

	Object obj;
	obj.type = PWR_OBJ_NODE;
	PWR_Time now;
	double value = 0;
	PWR_AttrName type = PWR_ATTR_POWER;
	int ret = PWR_ObjAttrGetValue((void *)&obj, type, (void *)&value, &now);
	printf("The node power is %lf Watts and the time is %ld\n", value, now);

    Object mem;
	mem.type = PWR_OBJ_MEM;
	ret = PWR_ObjAttrGetValue(&mem, type, &value, &now);
	printf("Memory power is %lf Watts and the time is %ld\n", value, now);


	Object limit;
	limit.type = PWR_OBJ_NODE;
	int limit_value = 600;
	ret = PWR_ObjAttrSetValue(&limit, type, &limit_value);
	if(ret == PWR_RET_SUCCESS) {
		printf("Power Limit Set Success!\n");
	} else {printf("Power Limit Set Failed!\n");}

	Object socket0;
	socket0.type = PWR_OBJ_SOCKET;
	socket0.misc = 0;
	ret = PWR_ObjAttrGetValue(&socket0, type, &value, &now );
	if(ret == PWR_RET_SUCCESS) {
		printf("Socket 0 power is  %lf watts and the time is %ld\n", value, now);
	}

	Object socket1;
    socket1.type = PWR_OBJ_SOCKET;
    socket1.misc = 1;
    ret = PWR_ObjAttrGetValue(&socket1, type, &value, &now );
	if(ret == PWR_RET_SUCCESS) {
		  printf("Socket 1 power is  %lf watts and the time is %ld\n", value, now);
    }

	return 0;

}
