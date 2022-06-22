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

if __name__ == "__main__":
    v = pyVariorum.variorum()
    runtests(v)
