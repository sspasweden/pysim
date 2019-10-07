#pragma once
#include <vector>
#include <map>
#include <memory>

#include "Variable.hpp"

#include "PysimTypes.hpp"

#include "boost/variant.hpp"

#include <Eigen/Dense>

namespace pysim {

struct VariablePrivate{
    std::map<std::string, double* > scalars;
    std::vector<std::string> scalarNames;
    std::map<std::string, pysim::vector* > vectors;
    std::vector<std::string> vectorNames;
    std::map<std::string, pysim::matrix* > matrices;
    std::vector<std::string> matrixNames;
    std::map<std::string, std::string> descriptions;
};

}
