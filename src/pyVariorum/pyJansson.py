from ctypes import *

class jansson:

    def __init__(self):
        try:
            self.jansson_c = CDLL("libjansson.so")
        except:
            print("\nJansson shared library not found. Please check path to .so file")

        # Jansson Key Functions    
        self.json_object = self.jansson_c.json_object
        self.json_object.restype = POINTER(c_void_p)
   
        #self.json_decref = self.jansson_c.json_decref
        #self.json_decref.argtypes = [POINTER(c_void_p)]
        #self.json_decref.restype = None
 
        self.json_dumps = self.jansson_c.json_dumps
        self.json_dumps.argtypes = [POINTER(c_void_p)]
        self.json_dumps.restype = c_char_p

