// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <variorum.h>
#include <jansson.h>

#ifdef SECOND_RUN
static inline double do_work(int input)
{
    int i;
    double result = (double)input;

    for (i = 0; i < 100000; i++)
    {
        result += i * result;
    }

    return result;
}
#endif

int main(void)
{
    int ret;
    json_t *my_power_obj = NULL;
    char *s = NULL;
#ifdef SECOND_RUN
    int i;
    int size = 1E4;
    double x = 0.0;
#endif

    my_power_obj =
        json_object(); // Create JSON object and pass to variorum API as reference.
    ret = variorum_get_node_power_json(my_power_obj);
    if (ret != 0)
    {
        printf("First run: JSON get node power failed!\n");
    }
    s = json_dumps(my_power_obj, 0);
    puts(s);

#ifdef SECOND_RUN
    for (i = 0; i < size; i++)
    {
        x += do_work(i);
    }
    ret = variorum_get_node_power_json(my_power_obj);
    if (ret != 0)
    {
        printf("Second run: JSON get node power failed!\n");
    }
#endif

    s = json_dumps(my_power_obj, 0);
    puts(s);
    json_decref(my_power_obj);
    return ret;
}
