import pyVariorum
    
def runtests(v):
    print("\n=== Running Variorum Print Power:")
    v.variorum_print_power()

    print("\n=== Running Variorum Print Power (Verbose):")
    v.variorum_print_verbose_power()

    print("\n=== Running Variorum Print Power Limit:")
    v.variorum_print_power_limit()

    print("\n=== Running Variorum Print Power Limit (Verbose):")
    v.variorum_print_verbose_power_limit()

    print("\n=== Running Variorum Cap Best Effort Node Power Limit:")
    v.variorum_cap_best_effort_node_power_limit(2000)
    print("\n######## Verifying with Variorum Print Power Limit:")
    v.variorum_print_power_limit()

#def runJSONTests(v)
#    print("\n=== Running Variorum Cap Best Effort Node Power Limit:")
#    v.variorum_get_node_power_json(my_power_obj)

if __name__ == "__main__":
    v = pyVariorum.variorum()
    runtests(v)
