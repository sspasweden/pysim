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

void CppSystem::setDiscrete(bool d) {
    d_ptr->isDiscrete = d;
}

void CppSystem::setNextUpdate(double t){
    d_ptr->nextUpdateTime = t;
}

}