from ctypes import create_unicode_buffer, addressof, c_char_p
import pyVariorum


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


def runJSONtests(v):
    print("\n=== Running Variorum JSON Get Node Power:")
    jsonbuf = create_unicode_buffer(1000)
    bufptr = c_char_p(addressof(jsonbuf))
    v.variorum_get_node_power_json(bufptr)
    print(bufptr.value.decode("utf-8"))
    # print(bufptr.value)


if __name__ == "__main__":
    v = pyVariorum.variorum()
    runtests(v)
    runJSONtests(v)
