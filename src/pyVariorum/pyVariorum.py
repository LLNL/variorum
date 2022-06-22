from ctypes import *

class variorum:

    def __init__(self):
        try:
            self.variorum_c = CDLL("libvariorum.so")
        except:
            print("\nVariorum shared library not found. Please check path to .so file")

    # Variorum Print Functions    
    def variorum_print_power(self):    
        ret = self.variorum_c.variorum_print_power()
        return ret
    
    def variorum_print_verbose_power(self):    
        ret = self.variorum_c.variorum_print_verbose_power()
        return ret
    
    def variorum_print_power_limit(self):    
        ret = self.variorum_c.variorum_print_power_limit()
        return ret
    
    def variorum_print_verbose_power_limit(self):    
        ret = self.variorum_c.variorum_print_verbose_power_limit()
        return ret

    # Variorum Cap Functions    
    def variorum_cap_best_effort_node_power_limit(self, node_power_limit):
        ret = self.variorum_c.variorum_cap_best_effort_node_power_limit(node_power_limit)
        return ret

    # Variorum JSON Functions    
#    def variorum_get_node_power_json(self, get_power_obj):
#        ret = self.variorum_c.variorum_get_node_power_json(byref(get_power_obj))
#        return ret
