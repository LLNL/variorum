from ctypes import *    
import pyVariorum
import pyJansson
    
def runtests(v):
    print("\n=== Running Variorum Print Power:")
    v.variorum_print_power()

    print("\n=== Running Variorum Print Power Limit:")
    v.variorum_print_power_limit()

    print("\n=== Running Variorum Print Power Limit (Verbose):")
    v.variorum_print_verbose_power_limit()

    print("\n=== Running Variorum Cap Best Effort Node Power Limit (input = 2000W):")
    v.variorum_cap_best_effort_node_power_limit(2000)
    print("\n######## Verifying with Variorum Print Power Limit:")
    v.variorum_print_power_limit()

def runJSONtests(v,j):
    print("\n=== Running Variorum JSON Get Node Power:")
    my_power_obj = j.json_object()
    v.variorum_get_node_power_json(my_power_obj)
    s = j.json_dumps(my_power_obj)
    print(s)
    #j.json_decref(my_power_obj) 
    

if __name__ == "__main__":
    v = pyVariorum.variorum()
    j = pyJansson.jansson()
    runtests(v)
    runJSONtests(v,j)
