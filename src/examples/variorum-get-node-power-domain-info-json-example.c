// Copyright 2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <stdio.h>

#include <variorum.h>
#include <jansson.h>

int main(void)
{
    int ret;
    json_t *my_domain_obj = NULL;
    char *s = NULL;

    // Create JSON object and pass to variorum API as reference.
    my_domain_obj = json_object();
    ret = variorum_get_node_power_domain_info_json(my_domain_obj);
    if (ret != 0)
    {
        printf("First run: JSON get node power domain information failed!\n");
    }
    s = json_dumps(my_domain_obj, 0);
    puts(s);

    json_decref(my_domain_obj);
    return ret;
}
