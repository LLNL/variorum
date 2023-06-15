#include <stdio.h>
#include "variorum-powerapi.h"


int main() {

	Object obj;
	obj.type = PWR_OBJ_NODE;
	PWR_Time now;
	uint64_t value = 0;
	PWR_AttrName type = PWR_ATTR_POWER;
	int ret = PWR_ObjAttrGetValue((void *)&obj, type, (void *)&value, &now);
	printf("The node power is %lf Watts and the time is %ld\n", value, now);


	return 0;

}

