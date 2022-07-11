from ctypes import *

class variorum:

    def __init__(self):
        try:
            self.variorum_c = CDLL("libvariorum.so")
        except:
            print("\nVariorum shared library not found. Please check path to .so file")

        # Variorum Print Functions    
        self.variorum_print_power = self.variorum_c.variorum_print_power
        self.variorum_print_power.restype = c_int

        self.variorum_print_power_limit = self.variorum_c.variorum_print_power_limit
        self.variorum_print_power_limit.restype = c_int

        self.variorum_print_verbose_power_limit = self.variorum_c.variorum_print_verbose_power_limit
        self.variorum_print_verbose_power_limit.restype = c_int
   
        self.variorum_cap_best_effort_node_power_limit = self.variorum_c.variorum_cap_best_effort_node_power_limit
        self.variorum_cap_best_effort_node_power_limit.argtypes = [c_int]
        self.variorum_cap_best_effort_node_power_limit.restype = c_int

        # Variorum JSON Functions    
        self.variorum_get_node_power_json = self.variorum_c.variorum_get_node_power_json
        self.variorum_get_node_power_json.argtypes = [POINTER(c_char_p)]
        self.variorum_get_node_power_json.restype = c_int

