#include <map>

#include "CommonSystemImpl.hpp"
#include "CommonSystemImpl_p.hpp"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <boost/format.hpp>

using std::string;


CommonSystemImpl::CommonSystemImpl() :
    d_ptr(new CommonSystemImplPrivate())
{
}

CommonSystemImpl::~CommonSystemImpl(){
}

std::map<std::string, std::string> CommonSystemImpl::getInputDescriptionMap(){
    return d_ptr->input_descriptions;
}

std::vector<std::string> CommonSystemImpl::getScalarInputNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->input_scalars.cbegin(); i != d_ptr->input_scalars.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::vector<std::string>  CommonSystemImpl::getInputVectorNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->input_vectors.cbegin(); i != d_ptr->input_vectors.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
};

void CommonSystemImpl::setInputVector(char* name, std::vector<double> value) {
    if (d_ptr->input_vectors.count(name) > 0) {
        auto bv = d_ptr->input_vectors[name];
        if (bv->size() != value.size()) {
            std::string errstr = str(boost::format("Size of %1% is %2%") % name % bv->size());
            throw std::invalid_argument(errstr);
        }
        std::copy(value.begin(), value.end(), bv->begin());
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }

}

std::vector<double> CommonSystemImpl::getInputVector(char* name) {
    if (d_ptr->input_vectors.count(name) > 0) {
        auto bv = d_ptr->input_vectors[name];
        std::vector<double> v(bv->size());
        std::copy(bv->begin(), bv->end(), v.begin());
        return v;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

std::vector<std::string> CommonSystemImpl::getOutputNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->output_scalars.cbegin(); i != d_ptr->output_scalars.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
};

std::vector<std::string> CommonSystemImpl::getOutputVectorNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->output_vectors.cbegin(); i != d_ptr->output_vectors.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

double CommonSystemImpl::getOutput(char* name) {
    if (d_ptr->output_scalars.count(name) < 1) {

        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->output_scalars.at(name));
};


std::vector<double> CommonSystemImpl::getOutputVector(char* name) {
    if (d_ptr->output_vectors.count(name) > 0) {
        auto bv = d_ptr->output_vectors[name];
        std::vector<double> v(bv->size());
        std::copy(bv->begin(), bv->end(), v.begin());
        return v;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

std::map<std::string, std::string> CommonSystemImpl::getOutputDescriptionMap() {
    return  d_ptr->output_descriptions;
}
