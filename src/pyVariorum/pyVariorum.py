from ctypes import *

class variorum:

    def __init__(self):
        try:
            self.variorum_c = CDLL("libvariorum.so")
        except:
            print("\nVariorum shared library not found. Please check path to .so file")
    
    def variorum_print_power(self):    
        return self.variorum_c.variorum_print_power()
    
    def variorum_print_verbose_power(self):    
        return self.variorum_c.variorum_print_verbose_power()
    
    def variorum_print_power_limit(self):    
        return self.variorum_c.variorum_print_power_limit()
    
    def variorum_print_verbose_power_limit(self):    
        return self.variorum_c.variorum_print_verbose_power_limit()
