#define _SCL_SECURE_NO_WARNINGS

#include "CppSystem.hpp"
#include "CommonSystemImpl_p.hpp"
#include "Variable_p.hpp"

#include <memory>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <iostream>

using std::string;
using std::shared_ptr;

namespace pysim {

CppSystem::CppSystem(){
}

CppSystem::~CppSystem() {
}


///////////////////////////////////////
//
//      Cpp Interface
//
///////////////////////////////////////

void CppSystem::state(double* state, const char* stateName, double* der, const char* derName,const char* description) {

    string stateNameString(stateName);
    boost::algorithm::trim(stateNameString);
    states.addScalar(stateNameString, state, description);

    string derNameString(derName);
    boost::algorithm::trim(derNameString);
    ders.addScalar(derNameString, der, description);

    d_ptr->state_to_der_map_scalars[stateNameString] = derNameString;

}

void CppSystem::state(pysim::vector* state, const char* stateName, pysim::vector* der, const char* derName, const char* description) {

    string stateNameString(stateName);
    boost::algorithm::trim(stateNameString);
    states.addVector(stateNameString, state, description);

    string derNameString(derName);
    boost::algorithm::trim(derNameString);
    ders.addVector(derNameString, der, description);

    d_ptr->state_to_der_map_vectors[stateNameString] = derNameString;
}

void CppSystem::state(pysim::matrix* state, const char* stateName, pysim::matrix* der, const char* derName, const char* description) {

    string stateNameString(stateName);
    boost::algorithm::trim(stateNameString);
    states.addMatrix(stateNameString, state, description);

    string derNameString(derName);
    boost::algorithm::trim(derNameString);
    ders.addMatrix(derNameString, der, description);

    d_ptr->state_to_der_map_matrices[stateNameString] = derNameString;
}


void CppSystem::input(double* var, const char* name, const char* description) {
    std::string str(name);
    boost::algorithm::trim(str);
    inputs.addScalar(str, var, description);
}

void CppSystem::par(std::vector<double>* var, const char* name, const char* description) {
    //TODO:: Make parameter
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_vectors[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::par(std::vector<std::vector<double>>* var, const char* name, const char* description) {
    //TODO:: Make parameter
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_matrices[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::input(pysim::vector* vars, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    inputs.addVector(str, vars, description);
}

void CppSystem::input(pysim::matrix* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    inputs.addMatrix(str, var, description);
}


void CppSystem::par(string* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_strings[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::par(std::map<string, double>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_maps[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::par(std::map<string, std::vector<double>>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_vector_maps[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::output(double* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    outputs.addScalar(str, var, description);
}

void CppSystem::output(pysim::vector* vars, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    outputs.addVector(str, vars, description);
}

void CppSystem::output(pysim::matrix* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    outputs.addMatrix(str, var, description);
}

void CppSystem::setDiscrete(bool d) {
    d_ptr->isDiscrete = d;
}

void CppSystem::setNextUpdate(double t){
    d_ptr->nextUpdateTime = t;
}

}