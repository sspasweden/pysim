from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp.pair cimport pair

cimport simulatablesystem
from connections cimport ConnectionHandler

cdef extern from "Variable.hpp" namespace "pysim":
    cdef cppclass Variable:
        vector[string] getMatrixNames()
        vector[vector[double]] getMatrix(char* name) except +
        void setMatrix(char*, vector[vector[double]]) except +
        void addMatrix(string, size_t, size_t, string) except +
        vector[string] getVectorNames()
        vector[double] getVector(char* name) except +
        void setVector(char*, vector[double]) except +
        void addVector(string, size_t, string) except +
        vector[string] getScalarNames()
        void setScalar(char*, double) except +
        double getScalar(char*) except +
        void addScalar(string, string) except +
        map[string,string] getDescriptionMap()

cdef extern from "CommonSystemImpl.hpp" namespace "pysim":
    cdef cppclass CommonSystemImpl(simulatablesystem.SimulatableSystemInterface):

        Variable inputs
        Variable outputs
        Variable states
        Variable ders
        ConnectionHandler connectionHandler
        vector[string] subsystem_names

        void __preSim() except +
        void __preStep() except +
        void __doStep(double) except +
        void __postStep() except +

        void add_subsystem(simulatablesystem.SimulatableSystemInterface* , string) except +
        simulatablesystem.SimulatableSystemInterface* get_subsystem(string) except +

        vector[string] getParNames[T]()
        T getPar[T](char*) except +
        void setPar[T](char* name, T value) except +
        map[string,string] getParDescriptionMap() except +

        void store(const char* name) except +
        StoreHandler* getStoreHandlerP()

        void add_compare_greater(char* comparename, double comparevalue) except +
        void add_compare_greater(char* comparename, double comparevalue, unsigned int col) except +
        void add_compare_greater(char* comparename, double comparevalue, unsigned int row, unsigned int col) except +
        void add_compare_smaller(char* comparename, double comparevalue) except +
        void add_compare_smaller(char* comparename, double comparevalue, unsigned int col) except +
        void add_compare_smaller(char* comparename, double comparevalue, unsigned int row, unsigned int col) except +

cdef extern from "StoreHandler.hpp" namespace "pysim":
    cdef cppclass StoreHandler:
        size_t getStoreSize()  except +
        size_t getStoreColumns(char* name) except +
        pair[size_t, size_t] getStoreRowColumns(char* name) except +
        void fillWithStore(char* name, double* p, size_t rows, size_t columns) except +
        void fillWithTime(double* p) except +
        void fillWithMatrices(char* name, double* p, size_t timesteps, size_t rows, size_t columns) except +
        vector[string] getStoreNames()
        vector[string] getStoreMatricesNames()
        void setStoreInterval(double interval)




cdef class CommonSystem(simulatablesystem.SimulatableSystem):
    cdef CommonSystemImpl * _c_s
    cdef dict _subsystems
    cpdef void _initialize(CommonSystem) except *
    cpdef void _evaluate(CommonSystem, double) except *

cdef class Parameters:
    cdef CommonSystemImpl* _c_sys
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)

cdef class PysimVars:
    cdef Variable* _var_p
    @staticmethod
    cdef _create(Variable* var_ptr)

cdef class Results:
    cdef StoreHandler* shp
    @staticmethod
    cdef _create(StoreHandler* ptr)
