#pragma once
#include <vector>
#include <map>

#include "CommonSystemImpl.hpp"
#include "Variable.hpp"

#include "StoreHandler.hpp"
#include "PysimTypes.hpp"


namespace pysim {

typedef std::string ParString;
typedef std::vector<double> ParVector;
typedef std::map<std::string, double> ParMap;
typedef std::map<std::string, std::vector<double>> ParVectorMap;
typedef std::vector<std::vector<double>> ParMatrix;


template <class T>
struct StateType {
    T stateValue;
    T derValue;
    std::string description;
};

struct CommonSystemImplPrivate {

    // Par containers
    std::map<std::string, ParString*> par_strings;
    std::map< std::string, ParMap* > par_maps;
    std::map<std::string, ParVectorMap* > par_vector_maps;
    std::map<std::string, ParVector* > par_vectors;
    std::map<std::string, ParMatrix* > par_matrices;
    std::map<std::string, std::string> par_descriptions;

    std::map<std::string, SimulatableSystemInterface*> subsystems;
    std::vector<SimulatableSystemInterface*> subsystems_vec;

    StoreHandler storeHandler;

    bool isDiscrete {false};
    double nextUpdateTime{0.0};

    std::vector<std::pair<double*, double > > compare_greater_vector;
    std::vector<std::pair<double*, double > > compare_smaller_vector;

};

}