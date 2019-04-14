#pragma once
#include <vector>
#include <map>
#include <memory>
#include "PysimTypes.hpp"


namespace pysim {

struct VariablePrivate;

class Variable{
public:
    Variable();
    std::vector<std::string> getScalarNames();
    std::vector<std::string> getVectorNames();
    std::vector<std::string> getMatrixNames();

    void setScalar(char* name, double value);
    void setVector(char* name, std::vector<double> value);
    void setMatrix(char* name, std::vector<std::vector<double>> value);

    double getScalar(char* name);
    std::vector<double> getVector(char* name);
    std::vector<std::vector<double>> getMatrix(char* name);

    void addScalar(std::string name, std::string desc);
    void add(std::string name, double* ptr, std::string desc);
    void addVector(std::string name, size_t length, std::string desc);
    void add(std::string name, pysim::vector* ptr, std::string desc);
    void addMatrix(std::string name, size_t rows, size_t cols, std::string desc);
    void add(std::string name, pysim::matrix* ptr, std::string desc);

    std::vector<double*> getPointers();

    std::map<std::string, std::string> getDescriptionMap();

    std::unique_ptr<VariablePrivate> d_ptr;
};

}
