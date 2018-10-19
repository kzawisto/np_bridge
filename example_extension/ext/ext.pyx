
from numpy cimport import_array

import_array()

cdef extern from "stuff.h" :
    cdef object cumsum_columns(object)
    cdef object produce_example()
    
    

def cumsum_col(obj):
    return cumsum_columns(obj)
def example():
    return produce_example()