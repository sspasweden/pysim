"""This module contains functionality for creating a cython or python
system for pysim. Both cython and python systems can inherit from the
Sys class in this module and then be added to a simulation.
"""

from collections import namedtuple
from libcpp.vector cimport vector
import warnings

import numpy as np
cimport numpy as np
from cythonsystem cimport CythonSystemImpl
from commonsystem cimport PysimVars
from commonsystem cimport Results

from connections cimport Connections

np.import_array()

cdef class Sys:
    """This class is meant to be inherited by python and cython systems.
    They can then be set up and after that used in a simulation together with
    other systems, both c++, python and cython systems.
    """
    def __cinit__(self):
        cdef CythonSystemImpl* _c_sys_local
        _c_sys_local = new CythonSystemImpl()
        self._c_sys = _c_sys_local
        self._c_s = _c_sys_local
        self._c_sys.sysp = <void*> self
        self._SimulatableSystemInterface_p = _c_sys_local

        self.inputs = PysimVars._create(&_c_sys_local.inputs)
        self.outputs = PysimVars._create(&_c_sys_local.outputs)
        self.states = PysimVars._create(&_c_sys_local.states)
        self.ders = PysimVars._create(&_c_sys_local.ders)
        self.connections = Connections._create(&_c_sys_local.connectionHandler)
        self.res = Results._create(_c_sys_local.getStoreHandlerP())
        self.stores = []

    def __dealloc__(self):
        del self._c_sys

    cpdef void pre_step(self):
        pass

    cpdef void do_step(self, double time):
        pass

    cpdef void post_step(self):
        pass


cdef api void pre_step_callback(void* sys):
    cdef Sys s
    s = <Sys> (sys)
    s.pre_step()

cdef api void step_callback(void* sys, double time):
    cdef Sys s
    s = <Sys> (sys)
    s.do_step(time)

cdef api void post_step_callback(void* sys):
    cdef Sys s
    s = <Sys> (sys)
    s.post_step()

