from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

from commonsystem cimport CommonSystemImpl
from commonsystem cimport CommonSystem

cdef extern from "CythonSystemImpl.hpp" namespace "pysim":
    cdef cppclass CythonSystemImpl(CommonSystemImpl):
        void* sysp


cdef class Sys(CommonSystem):
    cdef CythonSystemImpl * _c_sys
    cpdef void pre_step(Sys)
    cpdef void do_step(Sys, double)
    cpdef void post_step(Sys)
